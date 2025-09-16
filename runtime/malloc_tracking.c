// Malloc tracking for DEBUG mode only
// This file provides memory leak detection and tracking for debugging

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdatomic.h>
#include <time.h>
#include <pthread.h>

#ifdef TSC_DEBUG_MEMORY

// Thread-safe hash table for tracking allocations
typedef struct AllocationEntry {
    void* ptr;
    size_t size;
    time_t timestamp;
    const char* file;
    int line;
    const char* function;
    struct AllocationEntry* next;
} AllocationEntry;

typedef struct {
    AllocationEntry** buckets;
    size_t bucket_count;
    _Atomic size_t total_allocations;
    _Atomic size_t total_deallocations;
    _Atomic size_t peak_allocations;
    _Atomic size_t current_allocations;
    pthread_mutex_t mutex;
} AllocationTracker;

static AllocationTracker g_tracker = {0};
static int g_tracker_initialized = 0;

// Hash function for pointer addresses
static size_t hash_ptr(void* ptr) {
    return ((uintptr_t)ptr) % g_tracker.bucket_count;
}

// Initialize the allocation tracker
static void init_tracker(void) {
    if (g_tracker_initialized) return;
    
    g_tracker.bucket_count = 1024; // Power of 2 for efficient hashing
    g_tracker.buckets = calloc(g_tracker.bucket_count, sizeof(AllocationEntry*));
    if (!g_tracker.buckets) {
        fprintf(stderr, "ERROR: Failed to initialize malloc tracker\n");
        return;
    }
    
    pthread_mutex_init(&g_tracker.mutex, NULL);
    g_tracker_initialized = 1;
    
    printf("DEBUG: Malloc tracker initialized with %zu buckets\n", g_tracker.bucket_count);
}

// Add allocation to tracker
static void track_allocation(void* ptr, size_t size, const char* file, int line, const char* function) {
    if (!g_tracker_initialized) init_tracker();
    
    pthread_mutex_lock(&g_tracker.mutex);
    
    AllocationEntry* entry = malloc(sizeof(AllocationEntry));
    if (!entry) {
        pthread_mutex_unlock(&g_tracker.mutex);
        return;
    }
    
    entry->ptr = ptr;
    entry->size = size;
    entry->timestamp = time(NULL);
    entry->file = file;
    entry->line = line;
    entry->function = function;
    
    size_t bucket = hash_ptr(ptr);
    entry->next = g_tracker.buckets[bucket];
    g_tracker.buckets[bucket] = entry;
    
    atomic_fetch_add(&g_tracker.total_allocations, 1);
    size_t current = atomic_fetch_add(&g_tracker.current_allocations, 1) + 1;
    
    // Update peak
    size_t peak = atomic_load(&g_tracker.peak_allocations);
    while (current > peak && !atomic_compare_exchange_weak(&g_tracker.peak_allocations, &peak, current)) {
        // Retry until we update peak or someone else updates it higher
    }
    
    pthread_mutex_unlock(&g_tracker.mutex);
}

// Remove allocation from tracker
static void untrack_allocation(void* ptr) {
    if (!g_tracker_initialized) return;
    
    pthread_mutex_lock(&g_tracker.mutex);
    
    size_t bucket = hash_ptr(ptr);
    AllocationEntry** entry_ptr = &g_tracker.buckets[bucket];
    
    while (*entry_ptr) {
        if ((*entry_ptr)->ptr == ptr) {
            AllocationEntry* to_free = *entry_ptr;
            *entry_ptr = (*entry_ptr)->next;
            free(to_free);
            
            atomic_fetch_add(&g_tracker.total_deallocations, 1);
            atomic_fetch_sub(&g_tracker.current_allocations, 1);
            
            pthread_mutex_unlock(&g_tracker.mutex);
            return;
        }
        entry_ptr = &(*entry_ptr)->next;
    }
    
    pthread_mutex_unlock(&g_tracker.mutex);
}

// Wrapped malloc function
void* __tsc_debug_malloc(size_t size, const char* file, int line, const char* function) {
    void* ptr = malloc(size);
    if (ptr) {
        track_allocation(ptr, size, file, line, function);
    }
    return ptr;
}

// Wrapped free function
void __tsc_debug_free(void* ptr, const char* file, int line, const char* function) {
    if (ptr) {
        untrack_allocation(ptr);
    }
    free(ptr);
}

// Wrapped calloc function
void* __tsc_debug_calloc(size_t num, size_t size, const char* file, int line, const char* function) {
    void* ptr = calloc(num, size);
    if (ptr) {
        track_allocation(ptr, num * size, file, line, function);
    }
    return ptr;
}

// Wrapped realloc function
void* __tsc_debug_realloc(void* ptr, size_t size, const char* file, int line, const char* function) {
    if (ptr) {
        untrack_allocation(ptr);
    }
    
    void* new_ptr = realloc(ptr, size);
    if (new_ptr) {
        track_allocation(new_ptr, size, file, line, function);
    }
    
    return new_ptr;
}

// Report memory leaks
void __tsc_report_memory_leaks(void) {
    if (!g_tracker_initialized) {
        printf("DEBUG: Malloc tracker not initialized\n");
        return;
    }
    
    pthread_mutex_lock(&g_tracker.mutex);
    
    size_t leaks = 0;
    size_t total_leaked_bytes = 0;
    
    printf("\n=== MEMORY LEAK REPORT ===\n");
    
    for (size_t i = 0; i < g_tracker.bucket_count; i++) {
        AllocationEntry* entry = g_tracker.buckets[i];
        while (entry) {
            leaks++;
            total_leaked_bytes += entry->size;
            
            printf("LEAK: %p (%zu bytes) allocated at %s:%d in %s()\n",
                   entry->ptr, entry->size, entry->file, entry->line, entry->function);
            
            entry = entry->next;
        }
    }
    
    printf("\nTotal leaks: %zu\n", leaks);
    printf("Total leaked bytes: %zu\n", total_leaked_bytes);
    printf("Peak allocations: %zu\n", atomic_load(&g_tracker.peak_allocations));
    printf("Total allocations: %zu\n", atomic_load(&g_tracker.total_allocations));
    printf("Total deallocations: %zu\n", atomic_load(&g_tracker.total_deallocations));
    printf("========================\n");
    
    pthread_mutex_unlock(&g_tracker.mutex);
}

// Get memory statistics
typedef struct {
    size_t total_allocations;
    size_t total_deallocations;
    size_t current_allocations;
    size_t peak_allocations;
    size_t total_leaked_bytes;
    size_t leak_count;
} TSC_MemoryStats;

TSC_MemoryStats __tsc_get_malloc_stats(void) {
    TSC_MemoryStats stats = {0};
    
    if (!g_tracker_initialized) return stats;
    
    pthread_mutex_lock(&g_tracker.mutex);
    
    stats.total_allocations = atomic_load(&g_tracker.total_allocations);
    stats.total_deallocations = atomic_load(&g_tracker.total_deallocations);
    stats.current_allocations = atomic_load(&g_tracker.current_allocations);
    stats.peak_allocations = atomic_load(&g_tracker.peak_allocations);
    
    // Count leaks
    for (size_t i = 0; i < g_tracker.bucket_count; i++) {
        AllocationEntry* entry = g_tracker.buckets[i];
        while (entry) {
            stats.leak_count++;
            stats.total_leaked_bytes += entry->size;
            entry = entry->next;
        }
    }
    
    pthread_mutex_unlock(&g_tracker.mutex);
    
    return stats;
}

// Cleanup tracker (called at program exit)
void __tsc_cleanup_malloc_tracker(void) {
    if (!g_tracker_initialized) return;
    
    pthread_mutex_lock(&g_tracker.mutex);
    
    // Free all remaining entries
    for (size_t i = 0; i < g_tracker.bucket_count; i++) {
        AllocationEntry* entry = g_tracker.buckets[i];
        while (entry) {
            AllocationEntry* next = entry->next;
            free(entry);
            entry = next;
        }
    }
    
    free(g_tracker.buckets);
    pthread_mutex_destroy(&g_tracker.mutex);
    
    g_tracker_initialized = 0;
}

#else // TSC_DEBUG_MEMORY not defined

// Stub implementations for non-debug builds
void* __tsc_debug_malloc(size_t size, const char* file, int line, const char* function) {
    (void)file; (void)line; (void)function;
    return malloc(size);
}

void __tsc_debug_free(void* ptr, const char* file, int line, const char* function) {
    (void)file; (void)line; (void)function;
    free(ptr);
}

void* __tsc_debug_calloc(size_t num, size_t size, const char* file, int line, const char* function) {
    (void)file; (void)line; (void)function;
    return calloc(num, size);
}

void* __tsc_debug_realloc(void* ptr, size_t size, const char* file, int line, const char* function) {
    (void)file; (void)line; (void)function;
    return realloc(ptr, size);
}

void __tsc_report_memory_leaks(void) {
    // No-op in non-debug builds
}

typedef struct {
    size_t total_allocations;
    size_t total_deallocations;
    size_t current_allocations;
    size_t peak_allocations;
    size_t total_leaked_bytes;
    size_t leak_count;
} TSC_MemoryStats;

TSC_MemoryStats __tsc_get_malloc_stats(void) {
    TSC_MemoryStats stats = {0};
    return stats;
}

void __tsc_cleanup_malloc_tracker(void) {
    // No-op in non-debug builds
}

#endif // TSC_DEBUG_MEMORY