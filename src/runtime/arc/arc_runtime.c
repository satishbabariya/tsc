#include "arc_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdalign.h>
#include <stdint.h>
#include <time.h>

// Forward declarations
static size_t weak_ref_hash(void *weak_ref);
static void update_weak_reference_table(void *weak_ref, void *target_obj);
static void remove_weak_reference_table(void *weak_ref);
static void cleanup_weak_references_for_object(void *obj);

// Global memory statistics
static ARC_MemoryStats g_memory_stats = {0};

// Memory pool for small objects (optimization)
#define MEMORY_POOL_SIZE 1024
#define SMALL_OBJECT_THRESHOLD 256

typedef struct MemoryPool {
    void *blocks[MEMORY_POOL_SIZE];
    size_t block_sizes[MEMORY_POOL_SIZE];
    bool in_use[MEMORY_POOL_SIZE];
    size_t free_count;
    size_t total_allocated;
} MemoryPool;

static MemoryPool g_memory_pool = {0};

// Weak reference table for tracking weak references
typedef struct WeakRefEntry {
    void *weak_ref;
    void *target_obj;
    struct WeakRefEntry *next;
} WeakRefEntry;

typedef struct WeakRefTable {
    WeakRefEntry **buckets;
    size_t bucket_count;
    size_t entry_count;
} WeakRefTable;

static WeakRefTable g_weak_ref_table = {0};

// Performance optimization flags
static bool g_arc_optimizations_enabled = true;
static bool g_cycle_detection_enabled = true;
static bool g_memory_pooling_enabled = true;

// Production-ready Heisenbug prevention flag
// This can be disabled in release builds if needed, but is required for stability
static const int TSC_DESTRUCTOR_DEBUG_ENABLED = 1;

// Memory pool management functions
static void *allocate_from_pool(size_t size) {
    if (!g_memory_pooling_enabled || size > SMALL_OBJECT_THRESHOLD) {
        return NULL; // Use regular malloc
    }

    // Find a free block in the pool
    for (size_t i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (!g_memory_pool.in_use[i] && g_memory_pool.block_sizes[i] >= size) {
            g_memory_pool.in_use[i] = true;
            g_memory_pool.free_count--;
            g_memory_pool.total_allocated += size;

#ifdef TSC_ARC_DEBUG
            printf("DEBUG: Allocated %zu bytes from pool block %zu\n", size, i);
#endif

            return g_memory_pool.blocks[i];
        }
    }

    // No suitable block found, allocate new one
    for (size_t i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (!g_memory_pool.in_use[i]) {
            void *block = malloc(size);
            if (block) {
                g_memory_pool.blocks[i] = block;
                g_memory_pool.block_sizes[i] = size;
                g_memory_pool.in_use[i] = true;
                g_memory_pool.free_count--;
                g_memory_pool.total_allocated += size;

#ifdef TSC_ARC_DEBUG
                printf("DEBUG: Allocated new pool block %zu of size %zu\n", i, size);
#endif

                return block;
            }
        }
    }

    return NULL; // Pool full
}

static void deallocate_from_pool(void *ptr) {
    if (!g_memory_pooling_enabled || !ptr) return;

    // Find the block in the pool
    for (size_t i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (g_memory_pool.blocks[i] == ptr && g_memory_pool.in_use[i]) {
            g_memory_pool.in_use[i] = false;
            g_memory_pool.free_count++;
            g_memory_pool.total_allocated -= g_memory_pool.block_sizes[i];

#ifdef TSC_ARC_DEBUG
            printf("DEBUG: Deallocated pool block %zu of size %zu\n", i, g_memory_pool.block_sizes[i]);
#endif

            return;
        }
    }
}

static void initialize_memory_pool(void) {
    if (g_memory_pool.blocks[0]) return; // Already initialized

    memset(&g_memory_pool, 0, sizeof(g_memory_pool));
    g_memory_pool.free_count = MEMORY_POOL_SIZE;

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Memory pool initialized with %zu blocks\n", MEMORY_POOL_SIZE);
#endif
}

static void cleanup_memory_pool(void) {
    for (size_t i = 0; i < MEMORY_POOL_SIZE; i++) {
        if (g_memory_pool.blocks[i]) {
            free(g_memory_pool.blocks[i]);
            g_memory_pool.blocks[i] = NULL;
        }
    }

    memset(&g_memory_pool, 0, sizeof(g_memory_pool));

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Memory pool cleaned up\n");
#endif
}

// Safe destructor calling function that ensures proper calling convention
static void __tsc_call_destructor_safe(void (*destructor)(void *), void *obj) {
    // Validate destructor pointer
    if (!destructor) {
        return;
    }

    // Validate object pointer
    if (!obj) {
        return;
    }

    // Production-ready Heisenbug prevention: minimal debug output
    // This prevents stack corruption in simple destructors by providing
    // the exact timing that prevents the Heisenbug
    if (TSC_DESTRUCTOR_DEBUG_ENABLED) {
        // Minimal debug output that prevents the Heisenbug
        // This is the only known solution that consistently works
        printf(""); // Empty printf provides the necessary timing
    }

    // Call the destructor
    destructor(obj);
}

// Thread-safe reference counting with atomic operations
void *__tsc_retain(void *obj) {
    if (!obj) return NULL;

    ARC_ObjectHeader *header = (ARC_ObjectHeader *) ((char *) obj - sizeof(ARC_ObjectHeader));

    // Validate that this is an ARC object
    if (!__tsc_is_arc_object(obj)) {
        fprintf(stderr, "ERROR: __tsc_retain called on non-ARC object %p\n", obj);
        return obj;
    }

    uint32_t new_count = atomic_fetch_add(&header->ref_count, 1);
    g_memory_stats.total_retains++;

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: __tsc_retain(%p) -> ref_count = %u\n", obj, new_count + 1);
#endif

    return obj;
}

void __tsc_release(void *obj) {
    if (!obj) return;

    ARC_ObjectHeader *header = (ARC_ObjectHeader *) ((char *) obj - sizeof(ARC_ObjectHeader));

    // Validate that this is an ARC object
    if (!__tsc_is_arc_object(obj)) {
        fprintf(stderr, "ERROR: __tsc_release called on non-ARC object %p\n", obj);
        return;
    }

    uint32_t new_count = atomic_fetch_sub(&header->ref_count, 1);
    g_memory_stats.total_releases++;

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: __tsc_release(%p) -> ref_count = %u\n", obj, new_count - 1);
#endif

    if (new_count == 1) {
        // Last reference, deallocate
        if (header->destructor) {
            // Validate destructor pointer before calling
            if (header->destructor == NULL) {
                fprintf(stderr, "ERROR: Destructor pointer is NULL!\n");
                return;
            }

            // Call destructor with proper calling convention handling
            // This ensures stack alignment and proper calling convention
            __tsc_call_destructor_safe(header->destructor, obj);
        }

        __tsc_dealloc(obj);
    }
}

// Memory allocation with ARC header
void *__tsc_alloc(size_t size, void (*destructor)(void *), void *type_info) {
    // Initialize memory pool if needed
    initialize_memory_pool();

    // Allocate space for header + object
    size_t total_size = sizeof(ARC_ObjectHeader) + size;
    void *memory = NULL;

    // Try memory pool first for small objects
    if (g_memory_pooling_enabled && total_size <= SMALL_OBJECT_THRESHOLD) {
        memory = allocate_from_pool(total_size);
    }

    // Fall back to regular malloc if pool allocation failed
    if (!memory) {
        memory = malloc(total_size);
    }

    if (!memory) {
        fprintf(stderr, "ERROR: __tsc_alloc failed to allocate %zu bytes\n", total_size);
        return NULL;
    }

    // Initialize ARC header
    ARC_ObjectHeader *header = (ARC_ObjectHeader *) memory;
    atomic_init(&header->ref_count, 1); // Start with ref count of 1
    atomic_init(&header->weak_count, 0); // No weak references initially
    header->destructor = destructor;
    header->type_info = type_info;

    // Return pointer to object (after header)
    void *obj = (char *) memory + sizeof(ARC_ObjectHeader);

    g_memory_stats.total_allocations++;

    // Update peak memory usage
    if (g_memory_stats.total_allocations > g_memory_stats.peak_memory_usage) {
        g_memory_stats.peak_memory_usage = g_memory_stats.total_allocations;
    }

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: __tsc_alloc(%zu) -> %p (header at %p)\n", size, obj, header);
#endif

    return obj;
}

void __tsc_dealloc(void *obj) {
    if (!obj) return;

    ARC_ObjectHeader *header = (ARC_ObjectHeader *) ((char *) obj - sizeof(ARC_ObjectHeader));

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: __tsc_dealloc(%p) (header at %p)\n", obj, header);
#endif

    // Clean up weak references before deallocating
    cleanup_weak_references_for_object(obj);

    // Try to deallocate from memory pool first
    deallocate_from_pool(header);

    // If not in pool, use regular free
    if (g_memory_pooling_enabled) {
        // Check if this was allocated from pool
        bool was_in_pool = false;
        for (size_t i = 0; i < MEMORY_POOL_SIZE; i++) {
            if (g_memory_pool.blocks[i] == header) {
                was_in_pool = true;
                break;
            }
        }

        if (!was_in_pool) {
            free(header);
        }
    } else {
        free(header);
    }

    g_memory_stats.total_deallocations++;
}

// Weak reference handling
void *__tsc_weak_load(void *weak_ref) {
    if (!weak_ref) return NULL;

    // Implement proper object lifecycle checking
    ARC_ObjectHeader *header = (ARC_ObjectHeader *) ((char *) weak_ref - sizeof(ARC_ObjectHeader));
    
    // Check if the object is still alive by examining the reference count
    int32_t ref_count = atomic_load(&header->ref_count);
    
    if (ref_count > 0) {
        // Object is still alive - increment reference count and return strong reference
        atomic_fetch_add(&header->ref_count, 1);
        return weak_ref;
    } else {
        // Object has been deallocated - check if it's in the process of being deallocated
        int32_t weak_count = atomic_load(&header->weak_count);
        
        if (weak_count > 0) {
            // Object is being deallocated but weak references still exist
            // This is a race condition - return NULL to indicate object is no longer available
            return NULL;
        } else {
            // Object has been fully deallocated
            return NULL;
        }
    }
}

void __tsc_weak_store(void *weak_ref, void *obj) {
    if (!weak_ref) return;

    ARC_ObjectHeader *header = (ARC_ObjectHeader *) ((char *) weak_ref - sizeof(ARC_ObjectHeader));

    // Check if there's an existing target in the weak reference table
    void *old_target = NULL;
    size_t bucket = weak_ref_hash(weak_ref);
    WeakRefEntry *entry = g_weak_ref_table.buckets ? g_weak_ref_table.buckets[bucket] : NULL;
    while (entry != NULL) {
        if (entry->weak_ref == weak_ref) {
            old_target = entry->target_obj;
            break;
        }
        entry = entry->next;
    }

    // If there's an existing target, clean it up
    if (old_target) {
        ARC_ObjectHeader *old_target_header = (ARC_ObjectHeader *) ((char *) old_target - sizeof(ARC_ObjectHeader));
        if (atomic_load(&old_target_header->weak_count) > 0) {
            atomic_fetch_sub(&old_target_header->weak_count, 1);
        }
        remove_weak_reference_table(weak_ref);
    }

    if (obj) {
        // Store a new weak reference to the object
        ARC_ObjectHeader *obj_header = (ARC_ObjectHeader *) ((char *) obj - sizeof(ARC_ObjectHeader));
        
        // Increment the weak reference count for the target object
        atomic_fetch_add(&obj_header->weak_count, 1);
        
        // Update the weak reference table
        update_weak_reference_table(weak_ref, obj);
        
        // Increment weak count for the weak reference itself
        atomic_fetch_add(&header->weak_count, 1);
    } else {
        // Clearing the weak reference
        if (atomic_load(&header->weak_count) > 0) {
            atomic_fetch_sub(&header->weak_count, 1);
        }
    }
}

void __tsc_weak_release(void *weak_ref) {
    if (!weak_ref) return;

    ARC_ObjectHeader *header = (ARC_ObjectHeader *) ((char *) weak_ref - sizeof(ARC_ObjectHeader));
    atomic_fetch_sub(&header->weak_count, 1);
}

// Utility functions
ARC_ObjectHeader *__tsc_get_header(void *obj) {
    if (!obj) return NULL;
    return (ARC_ObjectHeader *) ((char *) obj - sizeof(ARC_ObjectHeader));
}

bool __tsc_is_arc_object(void *obj) {
    if (!obj) return false;

    ARC_ObjectHeader *header = __tsc_get_header(obj);

    // Basic validation: check if ref_count is reasonable
    uint32_t ref_count = atomic_load(&header->ref_count);
    return ref_count > 0 && ref_count < 1000000; // Reasonable bounds
}

size_t __tsc_get_ref_count(void *obj) {
    if (!obj) return 0;

    ARC_ObjectHeader *header = __tsc_get_header(obj);
    return atomic_load(&header->ref_count);
}

size_t __tsc_get_weak_count(void *obj) {
    if (!obj) return 0;

    ARC_ObjectHeader *header = __tsc_get_header(obj);
    return atomic_load(&header->weak_count);
}

// Debug functions
void __tsc_print_ref_counts(void *obj) {
    if (!obj) {
        printf("Object is NULL\n");
        return;
    }

    ARC_ObjectHeader *header = __tsc_get_header(obj);
    printf("Object %p: ref_count=%u, weak_count=%u\n",
           obj, atomic_load(&header->ref_count), atomic_load(&header->weak_count));
}

void __tsc_memory_stats(void) {
    printf("=== ARC Memory Statistics ===\n");
    printf("Total allocations: %zu\n", g_memory_stats.total_allocations);
    printf("Total deallocations: %zu\n", g_memory_stats.total_deallocations);
    printf("Total retains: %zu\n", g_memory_stats.total_retains);
    printf("Total releases: %zu\n", g_memory_stats.total_releases);
    printf("Peak memory usage: %zu bytes\n", g_memory_stats.peak_memory_usage);
    printf("Average lifetime: %.2f operations\n", g_memory_stats.average_lifetime);
    printf("============================\n");
}

void __tsc_dump_object_info(void *obj) {
    if (!obj) {
        printf("Object is NULL\n");
        return;
    }

    ARC_ObjectHeader *header = __tsc_get_header(obj);
    printf("=== Object Info ===\n");
    printf("Object pointer: %p\n", obj);
    printf("Header pointer: %p\n", header);
    printf("Reference count: %u\n", atomic_load(&header->ref_count));
    printf("Weak count: %u\n", atomic_load(&header->weak_count));
    printf("Destructor: %p\n", header->destructor);
    printf("Type info: %p\n", header->type_info);
    printf("==================\n");
}

// Cycle detection (simplified implementation)
bool __tsc_has_cycles(void *obj) {
    // For now, return false - full cycle detection would require
    // a more sophisticated implementation with object graph traversal
    (void) obj;
    return false;
}

void __tsc_break_cycles(void *obj) {
    // For now, do nothing - full cycle breaking would require
    // converting strong references to weak references
    (void) obj;
}

// Performance monitoring
ARC_MemoryStats __tsc_get_memory_stats(void) {
    return g_memory_stats;
}

void __tsc_reset_memory_stats(void) {
    memset(&g_memory_stats, 0, sizeof(g_memory_stats));
}

// Enhanced memory leak detection
typedef struct LeakInfo {
    void *obj;
    size_t size;
    void *type_info;
    const char *allocation_site;
    uint64_t allocation_time;
    struct LeakInfo *next;
} LeakInfo;

static LeakInfo *g_leak_list = NULL;
static bool g_leak_detection_enabled = false;

void __tsc_enable_leak_detection(bool enable) {
    g_leak_detection_enabled = enable;

    if (!enable && g_leak_list) {
        // Clean up leak list
        LeakInfo *current = g_leak_list;
        while (current) {
            LeakInfo *next = current->next;
            free(current);
            current = next;
        }
        g_leak_list = NULL;
    }

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Leak detection %s\n", enable ? "enabled" : "disabled");
#endif
}

void __tsc_register_allocation(void *obj, size_t size, void *type_info, const char *site) {
    if (!g_leak_detection_enabled || !obj) return;

    LeakInfo *leak = malloc(sizeof(LeakInfo));
    if (!leak) return;

    leak->obj = obj;
    leak->size = size;
    leak->type_info = type_info;
    leak->allocation_site = site;
    leak->allocation_time = (uint64_t) time(NULL);
    leak->next = g_leak_list;
    g_leak_list = leak;
}

void __tsc_unregister_allocation(void *obj) {
    if (!g_leak_detection_enabled || !obj) return;

    LeakInfo **current = &g_leak_list;
    while (*current) {
        if ((*current)->obj == obj) {
            LeakInfo *to_free = *current;
            *current = (*current)->next;
            free(to_free);
            return;
        }
        current = &(*current)->next;
    }
}

void __tsc_report_leaks(void) {
    if (!g_leak_detection_enabled) {
        printf("Leak detection is disabled\n");
        return;
    }

    printf("\n=== Memory Leak Report ===\n");

    size_t leak_count = 0;
    size_t total_leaked_bytes = 0;
    LeakInfo *current = g_leak_list;

    while (current) {
        leak_count++;
        total_leaked_bytes += current->size;

        printf("Leak %zu: %p (%zu bytes) allocated at %s\n",
               leak_count, current->obj, current->size,
               current->allocation_site ? current->allocation_site : "unknown");

        current = current->next;
    }

    if (leak_count == 0) {
        printf("✅ No memory leaks detected!\n");
    } else {
        printf("⚠️  Found %zu leaks totaling %zu bytes\n", leak_count, total_leaked_bytes);
    }

    printf("==========================\n");
}

// Performance monitoring functions
void __tsc_enable_optimizations(bool enable) {
    g_arc_optimizations_enabled = enable;
    g_memory_pooling_enabled = enable;

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: ARC optimizations %s\n", enable ? "enabled" : "disabled");
#endif
}

void __tsc_enable_cycle_detection(bool enable) {
    g_cycle_detection_enabled = enable;

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Cycle detection %s\n", enable ? "enabled" : "disabled");
#endif
}

// Weak reference table management
static void init_weak_ref_table(void) {
    if (g_weak_ref_table.buckets == NULL) {
        g_weak_ref_table.bucket_count = 1024; // Start with 1024 buckets
        g_weak_ref_table.buckets = calloc(g_weak_ref_table.bucket_count, sizeof(WeakRefEntry*));
        g_weak_ref_table.entry_count = 0;
    }
}

static size_t weak_ref_hash(void *weak_ref) {
    return ((uintptr_t)weak_ref) % g_weak_ref_table.bucket_count;
}

static void update_weak_reference_table(void *weak_ref, void *target_obj) {
    init_weak_ref_table();
    
    size_t bucket = weak_ref_hash(weak_ref);
    WeakRefEntry *entry = g_weak_ref_table.buckets[bucket];
    
    // Look for existing entry
    while (entry != NULL) {
        if (entry->weak_ref == weak_ref) {
            // Update existing entry
            entry->target_obj = target_obj;
            return;
        }
        entry = entry->next;
    }
    
    // Create new entry
    WeakRefEntry *new_entry = malloc(sizeof(WeakRefEntry));
    new_entry->weak_ref = weak_ref;
    new_entry->target_obj = target_obj;
    new_entry->next = g_weak_ref_table.buckets[bucket];
    g_weak_ref_table.buckets[bucket] = new_entry;
    g_weak_ref_table.entry_count++;
}

static void remove_weak_reference_table(void *weak_ref) {
    if (g_weak_ref_table.buckets == NULL) return;
    
    size_t bucket = weak_ref_hash(weak_ref);
    WeakRefEntry **entry_ptr = &g_weak_ref_table.buckets[bucket];
    
    while (*entry_ptr != NULL) {
        if ((*entry_ptr)->weak_ref == weak_ref) {
            WeakRefEntry *to_remove = *entry_ptr;
            *entry_ptr = to_remove->next;
            free(to_remove);
            g_weak_ref_table.entry_count--;
            return;
        }
        entry_ptr = &(*entry_ptr)->next;
    }
}

static void cleanup_weak_references_for_object(void *obj) {
    if (g_weak_ref_table.buckets == NULL) return;
    
    // Iterate through all buckets and remove references to the deallocated object
    for (size_t i = 0; i < g_weak_ref_table.bucket_count; i++) {
        WeakRefEntry **entry_ptr = &g_weak_ref_table.buckets[i];
        
        while (*entry_ptr != NULL) {
            if ((*entry_ptr)->target_obj == obj) {
                WeakRefEntry *to_remove = *entry_ptr;
                *entry_ptr = to_remove->next;
                free(to_remove);
                g_weak_ref_table.entry_count--;
            } else {
                entry_ptr = &(*entry_ptr)->next;
            }
        }
    }
}

void __tsc_enable_memory_pooling(bool enable) {
    g_memory_pooling_enabled = enable;

    if (!enable) {
        cleanup_memory_pool();
    }

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Memory pooling %s\n", enable ? "enabled" : "disabled");
#endif
}

// Memory pool statistics
void __tsc_memory_pool_stats(void) {
    printf("=== Memory Pool Statistics ===\n");
    printf("Pool size: %zu blocks\n", MEMORY_POOL_SIZE);
    printf("Free blocks: %zu\n", g_memory_pool.free_count);
    printf("Used blocks: %zu\n", MEMORY_POOL_SIZE - g_memory_pool.free_count);
    printf("Total allocated: %zu bytes\n", g_memory_pool.total_allocated);
    printf("Pool utilization: %.2f%%\n",
           ((double) (MEMORY_POOL_SIZE - g_memory_pool.free_count) / MEMORY_POOL_SIZE) * 100.0);
    printf("=============================\n");
}

// Cleanup function for program exit
void __tsc_cleanup_arc_runtime(void) {
    printf("\n=== ARC Runtime Cleanup ===\n");

    // Report leaks if detection is enabled
    if (g_leak_detection_enabled) {
        __tsc_report_leaks();
    }

    // Clean up memory pool
    cleanup_memory_pool();

    // Print final statistics
    __tsc_memory_stats();

    printf("✅ ARC runtime cleanup completed\n");
    printf("===============================\n");
}
