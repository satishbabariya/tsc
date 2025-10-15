#ifndef TSC_MEMORY_AUDIT_H
#define TSC_MEMORY_AUDIT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Memory audit structure to track memory operations
typedef struct {
    size_t total_allocations;
    size_t total_deallocations;
    size_t current_allocated_bytes;
    size_t peak_allocated_bytes;
    size_t stack_depth;
    bool memory_leak_detected;
} MemoryAuditInfo;

// Memory audit functions
void memory_audit_init(void);
void memory_audit_cleanup(void);
MemoryAuditInfo* memory_audit_get_info(void);
void memory_audit_record_allocation(size_t size);
void memory_audit_record_deallocation(size_t size);
void memory_audit_record_stack_push(void);
void memory_audit_record_stack_pop(void);
void memory_audit_print_summary(void);

// Assertion functions for testing
void assert_true(bool condition, const char* message);
void assert_false(bool condition, const char* message);
void assert_equals_int(int32_t expected, int32_t actual, const char* message);
void assert_equals_double(double expected, double actual, const char* message);
void assert_equals_string(const char* expected, const char* actual, const char* message);
void assert_not_null(void* ptr, const char* message);
void assert_null(void* ptr, const char* message);
void assert_memory_safe(void* ptr, size_t size, const char* message);

// Test result tracking
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
    bool test_suite_passed;
} TestResults;

TestResults* get_test_results(void);
void reset_test_results(void);
void print_test_summary(void);

#ifdef __cplusplus
}
#endif

#endif // TSC_MEMORY_AUDIT_H