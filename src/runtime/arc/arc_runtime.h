#ifndef TSC_ARC_RUNTIME_H
#define TSC_ARC_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ARC object header structure
typedef struct {
    _Atomic uint32_t ref_count;        // Reference count (atomic)
    _Atomic uint32_t weak_count;       // Weak reference count (atomic)
    void (*destructor)(void*); // Destructor function
    void* type_info;          // Type information for debugging
} ARC_ObjectHeader;

// ARC runtime functions
void* __tsc_retain(void* obj);
void __tsc_release(void* obj);
void* __tsc_weak_load(void* weak_ref);
void __tsc_weak_store(void* weak_ref, void* obj);
void __tsc_weak_release(void* weak_ref);

// Memory management
void* __tsc_alloc(size_t size, void (*destructor)(void*), void* type_info);
void __tsc_dealloc(void* obj);

// Cycle detection
bool __tsc_has_cycles(void* obj);
void __tsc_break_cycles(void* obj);
void __tsc_cleanup_cycle_detection(void);
void __tsc_dump_cycle_context(void);

// Debug functions
void __tsc_print_ref_counts(void* obj);
void __tsc_memory_stats(void);
void __tsc_dump_object_info(void* obj);

// Utility functions
ARC_ObjectHeader* __tsc_get_header(void* obj);
bool __tsc_is_arc_object(void* obj);
size_t __tsc_get_ref_count(void* obj);
size_t __tsc_get_weak_count(void* obj);

// Weak reference table functions
void __tsc_register_weak_ref(void* weak_ref, void* target_obj);
void __tsc_unregister_weak_ref(void* weak_ref);
void* __tsc_weak_ref_get_target(void* weak_ref);
bool __tsc_is_weak_ref_valid(void* weak_ref);
void __tsc_cleanup_weak_refs(void* target_obj);
void __tsc_clear_all_weak_refs(void);

// Performance monitoring
typedef struct {
    size_t total_allocations;
    size_t total_deallocations;
    size_t total_retains;
    size_t total_releases;
    size_t peak_memory_usage;
    double average_lifetime;
} ARC_MemoryStats;

ARC_MemoryStats __tsc_get_memory_stats(void);
void __tsc_reset_memory_stats(void);

// Enhanced memory management functions
void __tsc_enable_leak_detection(bool enable);
void __tsc_register_allocation(void* obj, size_t size, void* type_info, const char* site);
void __tsc_unregister_allocation(void* obj);
void __tsc_report_leaks(void);

// Performance monitoring functions
void __tsc_enable_optimizations(bool enable);
void __tsc_enable_cycle_detection(bool enable);
void __tsc_enable_memory_pooling(bool enable);
void __tsc_memory_pool_stats(void);

// Cleanup function
void __tsc_cleanup_arc_runtime(void);

#ifdef __cplusplus
}
#endif

#endif // TSC_ARC_RUNTIME_H