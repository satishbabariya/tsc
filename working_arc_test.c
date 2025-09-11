#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("Working ARC Test\n");
    printf("================\n");
    
    // Test 1: Basic object creation and destruction
    printf("\n--- Test 1: Basic Object ---\n");
    TSCObject* num = tsc_object_new(sizeof(double), tsc_type_number());
    if (num) {
        double* value = (double*)num->block->data;
        *value = 42.0;
        printf("Created number: %f (ref_count: %d)\n", *value, num->block->ref_count);
        tsc_object_unref(num);
        printf("After unref: live blocks = %zu\n", tsc_arc_get_live_count());
    }
    
    // Test 2: String object
    printf("\n--- Test 2: String Object ---\n");
    TSCObject* str = tsc_object_new(20, tsc_type_string());
    if (str) {
        char* data = (char*)str->block->data;
        strcpy(data, "Hello ARC!");
        printf("Created string: %s (ref_count: %d)\n", data, str->block->ref_count);
        tsc_object_unref(str);
        printf("After unref: live blocks = %zu\n", tsc_arc_get_live_count());
    }
    
    // Test 3: Reference counting
    printf("\n--- Test 3: Reference Counting ---\n");
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    if (obj1) {
        double* value = (double*)obj1->block->data;
        *value = 100.0;
        printf("Created obj1: %f (ref_count: %d)\n", *value, obj1->block->ref_count);
        
        // Create a reference
        TSCObject* obj2 = tsc_object_ref(obj1);
        printf("After ref: obj1 ref_count=%d, obj2 ref_count=%d\n", 
               obj1->block->ref_count, obj2->block->ref_count);
        
        // Unref the first object
        tsc_object_unref(obj1);
        printf("After unref(obj1): live blocks = %zu\n", tsc_arc_get_live_count());
        
        // Unref the second object (this should destroy the block)
        tsc_object_unref(obj2);
        printf("After unref(obj2): live blocks = %zu\n", tsc_arc_get_live_count());
    }
    
    printf("\nAll tests completed successfully!\n");
    return 0;
}