#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>

// Test counter for validation
static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT(condition, message) do { \
    if (condition) { \
        printf("PASS: %s\n", message); \
        tests_passed++; \
    } else { \
        printf("FAIL: %s\n", message); \
        tests_failed++; \
    } \
} while(0)

// Test basic functionality
void test_basic_functionality() {
    printf("\n=== Test: Basic Functionality ===\n");
    
    // Test object creation
    TSCObject* obj = tsc_object_new(sizeof(double), tsc_type_number());
    ASSERT(obj != NULL, "Object creation");
    ASSERT(obj->block != NULL, "Object has block");
    ASSERT(obj->block->ref_count == 1, "Initial reference count");
    
    // Test data access
    double* value = (double*)obj->block->data;
    *value = 42.0;
    ASSERT(*value == 42.0, "Data access and modification");
    
    // Test cleanup
    tsc_object_unref(obj);
    ASSERT(tsc_arc_get_live_count() == 0, "Cleanup after unref");
}

// Test reference counting
void test_reference_counting() {
    printf("\n=== Test: Reference Counting ===\n");
    
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    ASSERT(obj1 != NULL, "Object creation");
    
    // Test reference creation
    TSCObject* obj2 = tsc_object_ref(obj1);
    ASSERT(obj2 != NULL, "Reference creation");
    ASSERT(obj1->block == obj2->block, "References point to same block");
    ASSERT(obj1 != obj2, "References are different objects");
    ASSERT(obj1->block->ref_count == 2, "Reference count after ref");
    
    // Test partial cleanup
    tsc_object_unref(obj1);
    ASSERT(tsc_arc_get_live_count() == 1, "Live count after first unref");
    ASSERT(obj2->block->ref_count == 1, "Reference count after first unref");
    
    // Test final cleanup
    tsc_object_unref(obj2);
    ASSERT(tsc_arc_get_live_count() == 0, "Live count after final unref");
}

// Test edge cases
void test_edge_cases() {
    printf("\n=== Test: Edge Cases ===\n");
    
    // Test NULL object handling
    tsc_object_unref(NULL);
    ASSERT(1, "NULL object unref handled gracefully");
    
    // Test NULL object reference
    TSCObject* null_ref = tsc_object_ref(NULL);
    ASSERT(null_ref == NULL, "NULL object ref returns NULL");
    
    // Test object with NULL block
    TSCObject* obj = malloc(sizeof(TSCObject));
    obj->block = NULL;
    tsc_object_unref(obj);
    ASSERT(1, "Object with NULL block handled gracefully");
    
    // Test multiple references
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    TSCObject* obj2 = tsc_object_ref(obj1);
    TSCObject* obj3 = tsc_object_ref(obj1);
    TSCObject* obj4 = tsc_object_ref(obj2);
    
    ASSERT(obj1->block->ref_count == 4, "Multiple references counted correctly");
    
    // Clean up in different order
    tsc_object_unref(obj3);
    tsc_object_unref(obj1);
    tsc_object_unref(obj4);
    tsc_object_unref(obj2);
    
    ASSERT(tsc_arc_get_live_count() == 0, "Cleanup with multiple references");
}

// Test string operations
void test_string_operations() {
    printf("\n=== Test: String Operations ===\n");
    
    TSCObject* str1 = tsc_object_new(20, tsc_type_string());
    TSCObject* str2 = tsc_object_new(20, tsc_type_string());
    
    ASSERT(str1 != NULL && str2 != NULL, "String object creation");
    
    char* data1 = (char*)str1->block->data;
    char* data2 = (char*)str2->block->data;
    strcpy(data1, "Hello ");
    strcpy(data2, "World!");
    
    TSCObject* result = tsc_string_concat(str1, str2);
    ASSERT(result != NULL, "String concatenation");
    
    char* result_data = (char*)result->block->data;
    ASSERT(strcmp(result_data, "Hello World!") == 0, "String concatenation result");
    
    // Clean up
    tsc_object_unref(str1);
    tsc_object_unref(str2);
    tsc_object_unref(result);
    
    ASSERT(tsc_arc_get_live_count() == 0, "String cleanup");
}

// Test error conditions
void test_error_conditions() {
    printf("\n=== Test: Error Conditions ===\n");
    
    // Test invalid type info
    TSCObject* obj = tsc_object_new(sizeof(double), NULL);
    ASSERT(obj == NULL, "NULL type info handled");
    
    // Test string concatenation with NULL
    TSCObject* str1 = tsc_object_new(20, tsc_type_string());
    TSCObject* result = tsc_string_concat(str1, NULL);
    ASSERT(result == NULL, "String concat with NULL handled");
    
    tsc_object_unref(str1);
}

// Thread safety test data
typedef struct {
    TSCObject* shared_obj;
    int thread_id;
    int iterations;
} thread_data_t;

// Thread function for concurrency testing
void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    for (int i = 0; i < data->iterations; i++) {
        // Create reference to shared object
        TSCObject* ref = tsc_object_ref(data->shared_obj);
        if (ref) {
            // Simulate some work
            usleep(1000); // 1ms
            tsc_object_unref(ref);
        }
    }
    
    return NULL;
}

// Test concurrency
void test_concurrency() {
    printf("\n=== Test: Concurrency ===\n");
    
    const int num_threads = 4;
    const int iterations_per_thread = 100;
    
    // Create shared object
    TSCObject* shared_obj = tsc_object_new(sizeof(double), tsc_type_number());
    ASSERT(shared_obj != NULL, "Shared object creation");
    
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];
    
    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].shared_obj = shared_obj;
        thread_data[i].thread_id = i;
        thread_data[i].iterations = iterations_per_thread;
        
        int result = pthread_create(&threads[i], NULL, thread_function, &thread_data[i]);
        ASSERT(result == 0, "Thread creation");
    }
    
    // Wait for threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Verify object is still valid
    ASSERT(shared_obj->block != NULL, "Shared object still valid after concurrency test");
    ASSERT(shared_obj->block->ref_count == 1, "Reference count correct after concurrency test");
    
    // Clean up
    tsc_object_unref(shared_obj);
    ASSERT(tsc_arc_get_live_count() == 0, "Cleanup after concurrency test");
}

// Test memory stress
void test_memory_stress() {
    printf("\n=== Test: Memory Stress ===\n");
    
    const int num_objects = 10000;
    TSCObject** objects = malloc(sizeof(TSCObject*) * num_objects);
    ASSERT(objects != NULL, "Object array allocation");
    
    // Create many objects
    for (int i = 0; i < num_objects; i++) {
        objects[i] = tsc_object_new(sizeof(double), tsc_type_number());
        ASSERT(objects[i] != NULL, "Object creation in stress test");
        
        double* value = (double*)objects[i]->block->data;
        *value = (double)i;
    }
    
    ASSERT(tsc_arc_get_live_count() == num_objects, "All objects created");
    
    // Create some references
    for (int i = 0; i < num_objects; i += 2) {
        TSCObject* ref = tsc_object_ref(objects[i]);
        ASSERT(ref != NULL, "Reference creation in stress test");
        tsc_object_unref(ref);
    }
    
    // Clean up all objects
    for (int i = 0; i < num_objects; i++) {
        tsc_object_unref(objects[i]);
    }
    
    ASSERT(tsc_arc_get_live_count() == 0, "All objects cleaned up");
    
    free(objects);
}

// Test circular references
void test_circular_references() {
    printf("\n=== Test: Circular References ===\n");
    
    TSCObject* obj1 = tsc_object_new(sizeof(TSCObject*), tsc_type_object());
    TSCObject* obj2 = tsc_object_new(sizeof(TSCObject*), tsc_type_object());
    
    ASSERT(obj1 != NULL && obj2 != NULL, "Circular reference objects created");
    
    // Create circular references
    TSCObject** obj1_ref = (TSCObject**)obj1->block->data;
    TSCObject** obj2_ref = (TSCObject**)obj2->block->data;
    *obj1_ref = tsc_object_ref(obj2);
    *obj2_ref = tsc_object_ref(obj1);
    
    ASSERT(obj1->block->ref_count == 2, "obj1 reference count");
    ASSERT(obj2->block->ref_count == 2, "obj2 reference count");
    
    // Clean up (this should leave circular references)
    tsc_object_unref(obj1);
    tsc_object_unref(obj2);
    
    // Force cleanup to prevent memory leak in tests
    tsc_arc_cleanup_all();
    
    ASSERT(tsc_arc_get_live_count() == 0, "Circular references cleaned up");
}

int main() {
    printf("ARC Validation Test Suite\n");
    printf("=========================\n");
    
    test_basic_functionality();
    test_reference_counting();
    test_edge_cases();
    test_string_operations();
    test_error_conditions();
    test_concurrency();
    test_memory_stress();
    test_circular_references();
    
    printf("\n=== Test Results ===\n");
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_failed);
    printf("Total tests: %d\n", tests_passed + tests_failed);
    
    if (tests_failed == 0) {
        printf("\n🎉 All validation tests passed!\n");
        return 0;
    } else {
        printf("\n❌ Some validation tests failed!\n");
        return 1;
    }
}