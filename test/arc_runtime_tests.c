#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../runtime/arc/arc_runtime.h"

// Test 1: Basic ARC Operations
void test_basic_arc_operations() {
    printf("=== Test 1: Basic ARC Operations ===\n");
    
    // Allocate an ARC object
    void* obj = __tsc_alloc(sizeof(int), NULL, NULL);
    assert(obj != NULL);
    
    // Check initial reference count
    size_t ref_count = __tsc_get_ref_count(obj);
    assert(ref_count == 1);
    printf("Initial reference count: %zu\n", ref_count);
    
    // Retain the object
    void* retained_obj = __tsc_retain(obj);
    assert(retained_obj == obj);
    
    ref_count = __tsc_get_ref_count(obj);
    assert(ref_count == 2);
    printf("Reference count after retain: %zu\n", ref_count);
    
    // Release the object
    __tsc_release(obj);
    ref_count = __tsc_get_ref_count(obj);
    assert(ref_count == 1);
    printf("Reference count after release: %zu\n", ref_count);
    
    // Release the object again (should deallocate)
    __tsc_release(obj);
    printf("Object deallocated\n");
    
    printf("Test 1 passed!\n\n");
}

// Test 2: Weak References
void test_weak_references() {
    printf("=== Test 2: Weak References ===\n");
    
    // Create a strong reference
    void* strong_obj = __tsc_alloc(sizeof(int), NULL, NULL);
    assert(strong_obj != NULL);
    
    // Create a weak reference
    void* weak_obj = __tsc_weak_load(strong_obj);
    assert(weak_obj == strong_obj);
    
    size_t weak_count = __tsc_get_weak_count(strong_obj);
    printf("Weak reference count: %zu\n", weak_count);
    
    // Release the strong reference
    __tsc_release(strong_obj);
    
    // Try to load from weak reference (should return NULL)
    void* weak_load_result = __tsc_weak_load(weak_obj);
    assert(weak_load_result == NULL);
    printf("Weak reference correctly returns NULL after strong reference is released\n");
    
    printf("Test 2 passed!\n\n");
}

// Test 3: Memory Statistics
void test_memory_statistics() {
    printf("=== Test 3: Memory Statistics ===\n");
    
    // Reset statistics
    __tsc_reset_memory_stats();
    
    // Create some objects
    void* obj1 = __tsc_alloc(sizeof(int), NULL, NULL);
    void* obj2 = __tsc_alloc(sizeof(int), NULL, NULL);
    void* obj3 = __tsc_alloc(sizeof(int), NULL, NULL);
    
    // Retain and release some objects
    __tsc_retain(obj1);
    __tsc_retain(obj2);
    __tsc_release(obj1);
    __tsc_release(obj2);
    __tsc_release(obj3);
    
    // Get statistics
    ARC_MemoryStats stats = __tsc_get_memory_stats();
    printf("Memory Statistics:\n");
    printf("  Total allocations: %zu\n", stats.total_allocations);
    printf("  Total deallocations: %zu\n", stats.total_deallocations);
    printf("  Total retains: %zu\n", stats.total_retains);
    printf("  Total releases: %zu\n", stats.total_releases);
    
    assert(stats.total_allocations == 3);
    assert(stats.total_deallocations == 3);
    assert(stats.total_retains == 2);
    assert(stats.total_releases == 3);
    
    printf("Test 3 passed!\n\n");
}

// Test 4: Object Validation
void test_object_validation() {
    printf("=== Test 4: Object Validation ===\n");
    
    // Test valid ARC object
    void* valid_obj = __tsc_alloc(sizeof(int), NULL, NULL);
    assert(__tsc_is_arc_object(valid_obj) == true);
    printf("Valid ARC object detected\n");
    
    // Test invalid object (NULL)
    assert(__tsc_is_arc_object(NULL) == false);
    printf("NULL object correctly rejected\n");
    
    // Test invalid object (regular malloc)
    void* invalid_obj = malloc(sizeof(int));
    assert(__tsc_is_arc_object(invalid_obj) == false);
    printf("Non-ARC object correctly rejected\n");
    free(invalid_obj);
    
    // Clean up
    __tsc_release(valid_obj);
    
    printf("Test 4 passed!\n\n");
}

// Test 5: Debug Functions
void test_debug_functions() {
    printf("=== Test 5: Debug Functions ===\n");
    
    void* obj = __tsc_alloc(sizeof(int), NULL, NULL);
    __tsc_retain(obj);
    
    printf("Object info:\n");
    __tsc_dump_object_info(obj);
    
    printf("Reference counts:\n");
    __tsc_print_ref_counts(obj);
    
    printf("Memory stats:\n");
    __tsc_memory_stats();
    
    __tsc_release(obj);
    __tsc_release(obj);
    
    printf("Test 5 passed!\n\n");
}

// Test 6: Cycle Detection (Placeholder)
void test_cycle_detection() {
    printf("=== Test 6: Cycle Detection ===\n");
    
    void* obj = __tsc_alloc(sizeof(int), NULL, NULL);
    
    // Test cycle detection (simplified implementation)
    bool has_cycles = __tsc_has_cycles(obj);
    printf("Cycle detection result: %s\n", has_cycles ? "cycles found" : "no cycles");
    
    // Test cycle breaking
    __tsc_break_cycles(obj);
    printf("Cycle breaking completed\n");
    
    __tsc_release(obj);
    
    printf("Test 6 passed!\n\n");
}

// Test 7: Stress Test
void test_stress_test() {
    printf("=== Test 7: Stress Test ===\n");
    
    const int num_objects = 1000;
    void** objects = malloc(num_objects * sizeof(void*));
    
    // Create many objects
    for (int i = 0; i < num_objects; i++) {
        objects[i] = __tsc_alloc(sizeof(int), NULL, NULL);
        assert(objects[i] != NULL);
    }
    
    printf("Created %d objects\n", num_objects);
    
    // Retain some objects randomly
    for (int i = 0; i < num_objects; i += 2) {
        __tsc_retain(objects[i]);
    }
    
    printf("Retained %d objects\n", num_objects / 2);
    
    // Release all objects
    for (int i = 0; i < num_objects; i++) {
        __tsc_release(objects[i]);
    }
    
    printf("Released all objects\n");
    
    free(objects);
    
    printf("Test 7 passed!\n\n");
}

// Test 8: Error Handling
void test_error_handling() {
    printf("=== Test 8: Error Handling ===\n");
    
    // Test retain on NULL
    void* null_retain = __tsc_retain(NULL);
    assert(null_retain == NULL);
    printf("Retain on NULL handled correctly\n");
    
    // Test release on NULL
    __tsc_release(NULL);
    printf("Release on NULL handled correctly\n");
    
    // Test weak load on NULL
    void* null_weak = __tsc_weak_load(NULL);
    assert(null_weak == NULL);
    printf("Weak load on NULL handled correctly\n");
    
    printf("Test 8 passed!\n\n");
}

// Main test runner
int main() {
    printf("ARC Runtime Test Suite\n");
    printf("======================\n\n");
    
    test_basic_arc_operations();
    test_weak_references();
    test_memory_statistics();
    test_object_validation();
    test_debug_functions();
    test_cycle_detection();
    test_stress_test();
    test_error_handling();
    
    printf("All ARC runtime tests passed!\n");
    return 0;
}