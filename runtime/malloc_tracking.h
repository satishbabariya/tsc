#ifndef TSC_MALLOC_TRACKING_H
#define TSC_MALLOC_TRACKING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Debug malloc tracking functions
// These wrap standard malloc/free functions with leak detection

void* __tsc_debug_malloc(size_t size, const char* file, int line, const char* function);
void __tsc_debug_free(void* ptr, const char* file, int line, const char* function);
void* __tsc_debug_calloc(size_t num, size_t size, const char* file, int line, const char* function);
void* __tsc_debug_realloc(void* ptr, size_t size, const char* file, int line, const char* function);

// Memory leak reporting
void __tsc_report_memory_leaks(void);
void __tsc_cleanup_malloc_tracker(void);

// Memory statistics
typedef struct {
    size_t total_allocations;
    size_t total_deallocations;
    size_t current_allocations;
    size_t peak_allocations;
    size_t total_leaked_bytes;
    size_t leak_count;
} TSC_MemoryStats;

TSC_MemoryStats __tsc_get_malloc_stats(void);

// Convenience macros for automatic file/line/function tracking
#ifdef TSC_DEBUG_MEMORY
#define TSC_MALLOC(size) __tsc_debug_malloc(size, __FILE__, __LINE__, __FUNCTION__)
#define TSC_FREE(ptr) __tsc_debug_free(ptr, __FILE__, __LINE__, __FUNCTION__)
#define TSC_CALLOC(num, size) __tsc_debug_calloc(num, size, __FILE__, __LINE__, __FUNCTION__)
#define TSC_REALLOC(ptr, size) __tsc_debug_realloc(ptr, size, __FILE__, __LINE__, __FUNCTION__)
#else
#define TSC_MALLOC(size) malloc(size)
#define TSC_FREE(ptr) free(ptr)
#define TSC_CALLOC(num, size) calloc(num, size)
#define TSC_REALLOC(ptr, size) realloc(ptr, size)
#endif

#ifdef __cplusplus
}
#endif

#endif // TSC_MALLOC_TRACKING_H