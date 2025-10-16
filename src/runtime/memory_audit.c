#include "memory_audit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Global memory audit state
static MemoryAuditInfo g_audit_info = {0};
static TestResults g_test_results = {0};
static bool g_audit_initialized = false;

void memory_audit_init(void) {
    if (g_audit_initialized) return;
    
    memset(&g_audit_info, 0, sizeof(MemoryAuditInfo));
    memset(&g_test_results, 0, sizeof(TestResults));
    g_audit_initialized = true;
    
    printf("DEBUG: Memory audit system initialized\n");
}

void memory_audit_cleanup(void) {
    if (!g_audit_initialized) return;
    
    memory_audit_print_summary();
    g_audit_initialized = false;
}

MemoryAuditInfo* memory_audit_get_info(void) {
    if (!g_audit_initialized) {
        memory_audit_init();
    }
    return &g_audit_info;
}

void memory_audit_record_allocation(size_t size) {
    if (!g_audit_initialized) {
        memory_audit_init();
    }
    
    g_audit_info.total_allocations++;
    g_audit_info.current_allocated_bytes += size;
    
    if (g_audit_info.current_allocated_bytes > g_audit_info.peak_allocated_bytes) {
        g_audit_info.peak_allocated_bytes = g_audit_info.current_allocated_bytes;
    }
    
    printf("DEBUG: Memory allocated: %zu bytes (total: %zu)\n", 
           size, g_audit_info.current_allocated_bytes);
}

void memory_audit_record_deallocation(size_t size) {
    if (!g_audit_initialized) {
        memory_audit_init();
    }
    
    g_audit_info.total_deallocations++;
    if (g_audit_info.current_allocated_bytes >= size) {
        g_audit_info.current_allocated_bytes -= size;
    } else {
        g_audit_info.current_allocated_bytes = 0;
        g_audit_info.memory_leak_detected = true;
    }
    
    printf("DEBUG: Memory deallocated: %zu bytes (remaining: %zu)\n", 
           size, g_audit_info.current_allocated_bytes);
}

void memory_audit_record_stack_push(void) {
    if (!g_audit_initialized) {
        memory_audit_init();
    }
    
    g_audit_info.stack_depth++;
    printf("DEBUG: Stack push (depth: %zu)\n", g_audit_info.stack_depth);
}

void memory_audit_record_stack_pop(void) {
    if (!g_audit_initialized) {
        memory_audit_init();
    }
    
    if (g_audit_info.stack_depth > 0) {
        g_audit_info.stack_depth--;
    }
    printf("DEBUG: Stack pop (depth: %zu)\n", g_audit_info.stack_depth);
}

void memory_audit_print_summary(void) {
    if (!g_audit_initialized) return;
    
    printf("\n=== MEMORY AUDIT SUMMARY ===\n");
    printf("Total allocations: %zu\n", g_audit_info.total_allocations);
    printf("Total deallocations: %zu\n", g_audit_info.total_deallocations);
    printf("Current allocated bytes: %zu\n", g_audit_info.current_allocated_bytes);
    printf("Peak allocated bytes: %zu\n", g_audit_info.peak_allocated_bytes);
    printf("Current stack depth: %zu\n", g_audit_info.stack_depth);
    printf("Memory leak detected: %s\n", g_audit_info.memory_leak_detected ? "YES" : "NO");
    printf("============================\n\n");
}

// Assertion functions
void assert_true(bool condition, const char* message) {
    g_test_results.total_tests++;
    
    if (condition) {
        g_test_results.passed_tests++;
        printf("PASS: %s\n", message);
    } else {
        g_test_results.failed_tests++;
        printf("FAIL: %s\n", message);
    }
}

void assert_false(bool condition, const char* message) {
    assert_true(!condition, message);
}

void assert_equals_int(int32_t expected, int32_t actual, const char* message) {
    char full_message[256];
    snprintf(full_message, sizeof(full_message), "%s (expected: %d, actual: %d)", 
             message, expected, actual);
    assert_true(expected == actual, full_message);
}

void assert_equals_double(double expected, double actual, const char* message) {
    char full_message[256];
    // Use epsilon comparison for floating point
    double epsilon = 1e-9;
    bool equal = fabs(expected - actual) < epsilon;
    snprintf(full_message, sizeof(full_message), "%s (expected: %.6g, actual: %.6g)", 
             message, expected, actual);
    assert_true(equal, full_message);
}

void assert_equals_string(const char* expected, const char* actual, const char* message) {
    char full_message[256];
    bool equal = (expected == NULL && actual == NULL) || 
                 (expected != NULL && actual != NULL && strcmp(expected, actual) == 0);
    snprintf(full_message, sizeof(full_message), "%s (expected: '%s', actual: '%s')", 
             message, expected ? expected : "NULL", actual ? actual : "NULL");
    assert_true(equal, full_message);
}

void assert_not_null(void* ptr, const char* message) {
    char full_message[256];
    snprintf(full_message, sizeof(full_message), "%s (pointer: %p)", message, ptr);
    assert_true(ptr != NULL, full_message);
}

void assert_null(void* ptr, const char* message) {
    char full_message[256];
    snprintf(full_message, sizeof(full_message), "%s (pointer: %p)", message, ptr);
    assert_true(ptr == NULL, full_message);
}

void assert_memory_safe(void* ptr, size_t size, const char* message) {
    char full_message[256];
    bool safe = (ptr != NULL) && (size > 0);
    snprintf(full_message, sizeof(full_message), "%s (ptr: %p, size: %zu)", 
             message, ptr, size);
    assert_true(safe, full_message);
}

// Test result tracking
TestResults* get_test_results(void) {
    return &g_test_results;
}

void reset_test_results(void) {
    memset(&g_test_results, 0, sizeof(TestResults));
}

void print_test_summary(void) {
    printf("\n=== TEST SUMMARY ===\n");
    printf("Total tests: %d\n", g_test_results.total_tests);
    printf("Passed: %d\n", g_test_results.passed_tests);
    printf("Failed: %d\n", g_test_results.failed_tests);
    printf("Success rate: %.1f%%\n", 
           g_test_results.total_tests > 0 ? 
           (100.0 * g_test_results.passed_tests / g_test_results.total_tests) : 0.0);
    
    g_test_results.test_suite_passed = (g_test_results.failed_tests == 0);
    printf("Test suite: %s\n", g_test_results.test_suite_passed ? "PASSED" : "FAILED");
    printf("===================\n\n");
}