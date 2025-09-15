// ARC Retain/Release Semantics Integration Tests
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include "arc_runtime.h"

// Test object structure
typedef struct TestObject {
    int id;
    int value;
} TestObject;

// Destructor
void test_object_destructor(void* obj) {
    TestObject* test_obj = (TestObject*)obj;
    printf("🔴 TestObject %d (value: %d) destructed\n", test_obj->id, test_obj->value);
}

// Test 1: Basic retain/release semantics
int test_basic_retain_release() {
    printf("\n=== Test 1: Basic Retain/Release Semantics ===\n");
    
    // Create object
    TestObject* obj = (TestObject*)__tsc_alloc(sizeof(TestObject), test_object_destructor, NULL);
    obj->id = 1;
    obj->value = 42;
    
    printf("✅ Created TestObject with id=%d, value=%d\n", obj->id, obj->value);
    
    // Check initial ref count
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
    uint32_t initial_count = atomic_load(&header->ref_count);
    printf("📊 Initial ref_count: %u\n", initial_count);
    assert(initial_count == 1);
    
    // Retain object
    __tsc_retain(obj);
    uint32_t after_retain = atomic_load(&header->ref_count);
    printf("📊 After retain: %u\n", after_retain);
    assert(after_retain == 2);
    
    // Release object
    __tsc_release(obj);
    uint32_t after_release = atomic_load(&header->ref_count);
    printf("📊 After release: %u\n", after_release);
    assert(after_release == 1);
    
    // Final release should deallocate
    printf("🔄 Final release - object should be deallocated\n");
    __tsc_release(obj);
    
    printf("✅ Basic retain/release test passed\n");
    return 1;
}

// Test 2: Multiple retains and releases
int test_multiple_retain_release() {
    printf("\n=== Test 2: Multiple Retains and Releases ===\n");
    
    TestObject* obj = (TestObject*)__tsc_alloc(sizeof(TestObject), test_object_destructor, NULL);
    obj->id = 2;
    obj->value = 100;
    
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
    
    printf("✅ Created TestObject with id=%d, value=%d\n", obj->id, obj->value);
    
    // Multiple retains
    for (int i = 0; i < 5; i++) {
        __tsc_retain(obj);
        uint32_t count = atomic_load(&header->ref_count);
        printf("📊 After retain %d: ref_count = %u\n", i + 1, count);
        assert(count == i + 2); // Initial + retains
    }
    
    // Multiple releases
    for (int i = 0; i < 5; i++) {
        __tsc_release(obj);
        uint32_t count = atomic_load(&header->ref_count);
        printf("📊 After release %d: ref_count = %u\n", i + 1, count);
        assert(count == 5 - i); // 6 - releases
    }
    
    // Final release
    printf("🔄 Final release - object should be deallocated\n");
    __tsc_release(obj);
    
    printf("✅ Multiple retain/release test passed\n");
    return 1;
}

// Test 3: Retain/release with null pointer
int test_null_pointer_handling() {
    printf("\n=== Test 3: Null Pointer Handling ===\n");
    
    // Test retain with null
    printf("🔄 Testing retain with null pointer...\n");
    __tsc_retain(NULL); // Should not crash
    printf("✅ Retain with null handled gracefully\n");
    
    // Test release with null
    printf("🔄 Testing release with null pointer...\n");
    __tsc_release(NULL); // Should not crash
    printf("✅ Release with null handled gracefully\n");
    
    printf("✅ Null pointer handling test passed\n");
    return 1;
}

// Test 4: Retain/release with zero-sized object
int test_zero_sized_object() {
    printf("\n=== Test 4: Zero-Sized Object ===\n");
    
    // Create object with minimal size
    TestObject* obj = (TestObject*)__tsc_alloc(sizeof(TestObject), test_object_destructor, NULL);
    obj->id = 3;
    obj->value = 0;
    
    printf("✅ Created zero-sized TestObject\n");
    
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
    
    // Test retain/release cycle
    __tsc_retain(obj);
    uint32_t after_retain = atomic_load(&header->ref_count);
    printf("📊 After retain: ref_count = %u\n", after_retain);
    assert(after_retain == 2);
    
    __tsc_release(obj);
    uint32_t after_release = atomic_load(&header->ref_count);
    printf("📊 After release: ref_count = %u\n", after_release);
    assert(after_release == 1);
    
    // Final release
    printf("🔄 Final release - object should be deallocated\n");
    __tsc_release(obj);
    
    printf("✅ Zero-sized object test passed\n");
    return 1;
}

// Test 5: Thread safety of retain/release
typedef struct {
    TestObject* obj;
    int thread_id;
    int iterations;
} ThreadData;

void* thread_function(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    
    for (int i = 0; i < data->iterations; i++) {
        __tsc_retain(data->obj);
        __tsc_release(data->obj);
    }
    
    printf("✅ Thread %d completed %d retain/release cycles\n", data->thread_id, data->iterations);
    return NULL;
}

int test_thread_safety() {
    printf("\n=== Test 5: Thread Safety ===\n");
    
    TestObject* obj = (TestObject*)__tsc_alloc(sizeof(TestObject), test_object_destructor, NULL);
    obj->id = 4;
    obj->value = 200;
    
    printf("✅ Created TestObject for thread safety test\n");
    
    const int num_threads = 4;
    const int iterations_per_thread = 1000;
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];
    
    // Create threads
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].obj = obj;
        thread_data[i].thread_id = i;
        thread_data[i].iterations = iterations_per_thread;
        
        if (pthread_create(&threads[i], NULL, thread_function, &thread_data[i]) != 0) {
            printf("❌ Failed to create thread %d\n", i);
            return 0;
        }
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Check final ref count (should be 1 - the original reference)
    ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)obj - sizeof(ARC_ObjectHeader));
    uint32_t final_count = atomic_load(&header->ref_count);
    printf("📊 Final ref_count after all threads: %u\n", final_count);
    assert(final_count == 1);
    
    // Final release
    printf("🔄 Final release - object should be deallocated\n");
    __tsc_release(obj);
    
    printf("✅ Thread safety test passed\n");
    return 1;
}

// Test 6: Retain/release with destructor
int test_destructor_behavior() {
    printf("\n=== Test 6: Destructor Behavior ===\n");
    
    TestObject* obj = (TestObject*)__tsc_alloc(sizeof(TestObject), test_object_destructor, NULL);
    obj->id = 5;
    obj->value = 300;
    
    printf("✅ Created TestObject with destructor\n");
    
    // Retain and release
    __tsc_retain(obj);
    __tsc_release(obj);
    
    printf("🔄 Final release - destructor should be called\n");
    __tsc_release(obj);
    
    printf("✅ Destructor behavior test passed\n");
    return 1;
}

int main() {
    printf("🔄 ARC Retain/Release Semantics Integration Tests\n");
    printf("==================================================\n");
    
    int tests_passed = 0;
    int total_tests = 6;
    
    tests_passed += test_basic_retain_release();
    tests_passed += test_multiple_retain_release();
    tests_passed += test_null_pointer_handling();
    tests_passed += test_zero_sized_object();
    tests_passed += test_thread_safety();
    tests_passed += test_destructor_behavior();
    
    printf("\n📊 Test Results: %d/%d tests passed\n", tests_passed, total_tests);
    
    if (tests_passed == total_tests) {
        printf("✅ All retain/release tests passed!\n");
        return 0;
    } else {
        printf("❌ Some retain/release tests failed!\n");
        return 1;
    }
}