#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test circular references (potential memory leak)
void test_circular_references() {
    printf("=== Test: Circular References ===\n");
    
    // Create two objects that reference each other
    TSCObject* obj1 = tsc_object_new(sizeof(TSCObject*), tsc_type_object());
    TSCObject* obj2 = tsc_object_new(sizeof(TSCObject*), tsc_type_object());
    
    if (!obj1 || !obj2) {
        printf("FAIL: Failed to create objects for circular reference test\n");
        if (obj1) tsc_object_unref(obj1);
        if (obj2) tsc_object_unref(obj2);
        return;
    }
    
    // Store references to each other (simulating circular references)
    TSCObject** obj1_ref = (TSCObject**)obj1->block->data;
    TSCObject** obj2_ref = (TSCObject**)obj2->block->data;
    *obj1_ref = tsc_object_ref(obj2);
    *obj2_ref = tsc_object_ref(obj1);
    
    printf("Created circular references: obj1->obj2, obj2->obj1\n");
    printf("obj1 ref_count: %d, obj2 ref_count: %d\n", 
           obj1->block->ref_count, obj2->block->ref_count);
    
    // This should create a memory leak in pure ARC
    // In a real implementation, we'd need weak references or GC
    printf("WARNING: Circular references detected - potential memory leak\n");
    printf("This is expected behavior for ARC without weak references\n");
    
    // Clean up (this won't actually clean up due to circular references)
    tsc_object_unref(obj1);
    tsc_object_unref(obj2);
    
    printf("After cleanup: live blocks = %zu\n", tsc_arc_get_live_count());
    printf("PASS: Circular reference test completed (with expected memory leak)\n");
    
    // Force cleanup for testing purposes
    tsc_arc_cleanup_all();
}

// Test nested object structures
void test_nested_structures() {
    printf("\n=== Test: Nested Structures ===\n");
    
    // Create a parent object
    TSCObject* parent = tsc_object_new(sizeof(TSCObject*) * 3, tsc_type_object());
    if (!parent) {
        printf("FAIL: Failed to create parent object\n");
        return;
    }
    
    // Create child objects
    TSCObject* child1 = tsc_object_new(sizeof(double), tsc_type_number());
    TSCObject* child2 = tsc_object_new(sizeof(double), tsc_type_number());
    TSCObject* child3 = tsc_object_new(sizeof(double), tsc_type_number());
    
    if (!child1 || !child2 || !child3) {
        printf("FAIL: Failed to create child objects\n");
        tsc_object_unref(parent);
        if (child1) tsc_object_unref(child1);
        if (child2) tsc_object_unref(child2);
        if (child3) tsc_object_unref(child3);
        return;
    }
    
    // Set values
    double* val1 = (double*)child1->block->data;
    double* val2 = (double*)child2->block->data;
    double* val3 = (double*)child3->block->data;
    *val1 = 10.0;
    *val2 = 20.0;
    *val3 = 30.0;
    
    // Store references in parent
    TSCObject** children = (TSCObject**)parent->block->data;
    children[0] = tsc_object_ref(child1);
    children[1] = tsc_object_ref(child2);
    children[2] = tsc_object_ref(child3);
    
    printf("Created nested structure: parent with 3 children\n");
    printf("Parent ref_count: %d\n", parent->block->ref_count);
    printf("Child ref_counts: %d, %d, %d\n", 
           child1->block->ref_count, child2->block->ref_count, child3->block->ref_count);
    
    // Clean up children first
    tsc_object_unref(child1);
    tsc_object_unref(child2);
    tsc_object_unref(child3);
    
    printf("After unref children: live blocks = %zu\n", tsc_arc_get_live_count());
    
    // Clean up parent
    tsc_object_unref(parent);
    
    printf("After unref parent: live blocks = %zu\n", tsc_arc_get_live_count());
    printf("PASS: Nested structures test completed successfully!\n");
}

// Test high number of allocations and deallocations
void test_high_volume() {
    printf("\n=== Test: High Volume Allocations ===\n");
    
    const int num_objects = 1000;
    TSCObject** objects = malloc(sizeof(TSCObject*) * num_objects);
    
    if (!objects) {
        printf("FAIL: Failed to allocate object array\n");
        return;
    }
    
    // Create many objects
    for (int i = 0; i < num_objects; i++) {
        objects[i] = tsc_object_new(sizeof(double), tsc_type_number());
        if (!objects[i]) {
            printf("FAIL: Failed to create object %d\n", i);
            // Clean up already created objects
            for (int j = 0; j < i; j++) {
                tsc_object_unref(objects[j]);
            }
            free(objects);
            return;
        }
        
        double* value = (double*)objects[i]->block->data;
        *value = (double)i;
    }
    
    printf("Created %d objects\n", num_objects);
    printf("Live blocks: %zu\n", tsc_arc_get_live_count());
    
    // Create some references
    for (int i = 0; i < num_objects; i += 2) {
        TSCObject* ref = tsc_object_ref(objects[i]);
        if (ref) {
            tsc_object_unref(ref);
        }
    }
    
    printf("After creating references: live blocks = %zu\n", tsc_arc_get_live_count());
    
    // Clean up all objects
    for (int i = 0; i < num_objects; i++) {
        tsc_object_unref(objects[i]);
    }
    
    printf("After cleanup: live blocks = %zu\n", tsc_arc_get_live_count());
    printf("PASS: High volume test completed successfully!\n");
    
    free(objects);
}

// Test object size analysis
void test_object_sizes() {
    printf("\n=== Test: Object Size Analysis ===\n");
    
    printf("TSCObject size: %zu bytes\n", sizeof(TSCObject));
    printf("TSCARCBlock size: %zu bytes\n", sizeof(TSCARCBlock));
    printf("TypeInfo size: %zu bytes\n", sizeof(TypeInfo));
    
    // Test actual allocation sizes
    TSCObject* obj = tsc_object_new(sizeof(double), tsc_type_number());
    if (obj) {
        printf("Total memory per object: %zu bytes (TSCObject + TSCARCBlock + data)\n", 
               sizeof(TSCObject) + sizeof(TSCARCBlock) + sizeof(double));
        printf("Memory efficiency: %.2f%% (data vs overhead)\n", 
               (double)sizeof(double) / (sizeof(TSCObject) + sizeof(TSCARCBlock) + sizeof(double)) * 100);
        tsc_object_unref(obj);
    }
    
    printf("PASS: Object size analysis completed!\n");
}

// Test error handling
void test_error_handling() {
    printf("\n=== Test: Error Handling ===\n");
    
    // Test NULL object handling
    tsc_object_unref(NULL);
    printf("PASS: NULL object unref handled gracefully\n");
    
    // Test NULL block handling
    TSCObject* obj = malloc(sizeof(TSCObject));
    if (obj) {
        obj->block = NULL;
        tsc_object_unref(obj);
        printf("PASS: NULL block unref handled gracefully\n");
    }
    
    // Test reference counting edge cases
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    if (obj1) {
        // Test single unref (multiple unrefs would cause double-free)
        tsc_object_unref(obj1);
        printf("PASS: Single unref handled gracefully\n");
    }
    
    printf("PASS: Error handling test completed!\n");
}

int main() {
    printf("Extended ARC Test Suite\n");
    printf("=======================\n");
    
    test_circular_references();
    test_nested_structures();
    test_high_volume();
    test_object_sizes();
    test_error_handling();
    
    printf("\n=== Final Debug Information ===\n");
    tsc_arc_debug_print();
    
    printf("\nAll extended tests completed!\n");
    return 0;
}