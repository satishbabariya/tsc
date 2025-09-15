#include "arc_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Global memory statistics
static ARC_MemoryStats g_memory_stats = {0};

// Thread-safe reference counting with atomic operations
void* __tsc_retain(void* obj) {
    if (!obj) return NULL;
    
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
    
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

void __tsc_release(void* obj) {
    if (!obj) return;
    
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
    
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
            printf("DEBUG: Calling destructor %p for object %p\n", header->destructor, obj);
            header->destructor(obj);
        } else {
            printf("DEBUG: No destructor for object %p\n", obj);
        }
        __tsc_dealloc(obj);
    }
}

// Memory allocation with ARC header
void* __tsc_alloc(size_t size, void (*destructor)(void*), void* type_info) {
    // Allocate space for header + object
    size_t total_size = sizeof(ARC_ObjectHeader) + size;
    void* memory = malloc(total_size);
    
    if (!memory) {
        fprintf(stderr, "ERROR: __tsc_alloc failed to allocate %zu bytes\n", total_size);
        return NULL;
    }
    
    // Initialize ARC header
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)memory;
    atomic_init(&header->ref_count, 1);  // Start with ref count of 1
    atomic_init(&header->weak_count, 0);  // No weak references initially
    header->destructor = destructor;
    header->type_info = type_info;
    
    printf("DEBUG: __tsc_alloc allocated object %p with destructor %p\n", obj, destructor);
    
    // Return pointer to object (after header)
    void* obj = (char*)memory + sizeof(ARC_ObjectHeader);
    
    g_memory_stats.total_allocations++;
    
    #ifdef TSC_ARC_DEBUG
    printf("DEBUG: __tsc_alloc(%zu) -> %p (header at %p)\n", size, obj, header);
    #endif
    
    return obj;
}

void __tsc_dealloc(void* obj) {
    if (!obj) return;
    
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
    
    #ifdef TSC_ARC_DEBUG
    printf("DEBUG: __tsc_dealloc(%p) (header at %p)\n", obj, header);
    #endif
    
    // Free the entire block (header + object)
    free(header);
    
    g_memory_stats.total_deallocations++;
}

// Weak reference handling
void* __tsc_weak_load(void* weak_ref) {
    if (!weak_ref) return NULL;
    
    // For now, implement simple weak references
    // In a full implementation, this would check if the object is still alive
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)weak_ref - sizeof(ARC_ObjectHeader));
    
    if (atomic_load(&header->ref_count) > 0) {
        return __tsc_retain(weak_ref);
    }
    
    return NULL;
}

void __tsc_weak_store(void* weak_ref, void* obj) {
    if (!weak_ref) return;
    
    // For now, implement simple weak references
    // In a full implementation, this would update weak reference table
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)weak_ref - sizeof(ARC_ObjectHeader));
    
    if (obj) {
        atomic_fetch_add(&header->weak_count, 1);
    }
}

void __tsc_weak_release(void* weak_ref) {
    if (!weak_ref) return;
    
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)weak_ref - sizeof(ARC_ObjectHeader));
    atomic_fetch_sub(&header->weak_count, 1);
}

// Utility functions
ARC_ObjectHeader* __tsc_get_header(void* obj) {
    if (!obj) return NULL;
    return (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
}

bool __tsc_is_arc_object(void* obj) {
    if (!obj) return false;
    
    ARC_ObjectHeader* header = __tsc_get_header(obj);
    
    // Basic validation: check if ref_count is reasonable
    uint32_t ref_count = atomic_load(&header->ref_count);
    return ref_count > 0 && ref_count < 1000000; // Reasonable bounds
}

size_t __tsc_get_ref_count(void* obj) {
    if (!obj) return 0;
    
    ARC_ObjectHeader* header = __tsc_get_header(obj);
    return atomic_load(&header->ref_count);
}

size_t __tsc_get_weak_count(void* obj) {
    if (!obj) return 0;
    
    ARC_ObjectHeader* header = __tsc_get_header(obj);
    return atomic_load(&header->weak_count);
}

// Debug functions
void __tsc_print_ref_counts(void* obj) {
    if (!obj) {
        printf("Object is NULL\n");
        return;
    }
    
    ARC_ObjectHeader* header = __tsc_get_header(obj);
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

void __tsc_dump_object_info(void* obj) {
    if (!obj) {
        printf("Object is NULL\n");
        return;
    }
    
    ARC_ObjectHeader* header = __tsc_get_header(obj);
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
bool __tsc_has_cycles(void* obj) {
    // For now, return false - full cycle detection would require
    // a more sophisticated implementation with object graph traversal
    (void)obj;
    return false;
}

void __tsc_break_cycles(void* obj) {
    // For now, do nothing - full cycle breaking would require
    // converting strong references to weak references
    (void)obj;
}

// Performance monitoring
ARC_MemoryStats __tsc_get_memory_stats(void) {
    return g_memory_stats;
}

void __tsc_reset_memory_stats(void) {
    memset(&g_memory_stats, 0, sizeof(g_memory_stats));
}