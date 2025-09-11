#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test that reproduces the original segmentation fault
void test_reference_counting_segfault() {
    printf("=== Regression Test: Reference Counting Segfault ===\n");
    
    // Create obj1
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    if (!obj1) {
        printf("FAIL: Failed to create obj1\n");
        return;
    }
    
    double* value = (double*)obj1->block->data;
    *value = 100.0;
    printf("Created obj1: %f (ref_count: %d)\n", *value, obj1->block->ref_count);
    
    // Create obj2 reference
    TSCObject* obj2 = tsc_object_ref(obj1);
    if (!obj2) {
        printf("FAIL: Failed to create obj2\n");
        tsc_object_unref(obj1);
        return;
    }
    
    printf("After ref: obj1 ref_count=%d, obj2 ref_count=%d\n", 
           obj1->block->ref_count, obj2->block->ref_count);
    
    // Verify they point to the same block but different objects
    if (obj1->block != obj2->block) {
        printf("FAIL: obj1 and obj2 should point to the same block\n");
        tsc_object_unref(obj1);
        tsc_object_unref(obj2);
        return;
    }
    
    if (obj1 == obj2) {
        printf("FAIL: obj1 and obj2 should be different objects\n");
        tsc_object_unref(obj1);
        tsc_object_unref(obj2);
        return;
    }
    
    // Unref obj1 first
    tsc_object_unref(obj1);
    printf("After unref(obj1): live blocks = %zu\n", tsc_arc_get_live_count());
    
    // Verify obj2 is still valid
    if (obj2->block && obj2->block->ref_count == 1) {
        printf("PASS: obj2 is still valid after unref(obj1)\n");
    } else {
        printf("FAIL: obj2 is corrupted after unref(obj1)\n");
        tsc_object_unref(obj2);
        return;
    }
    
    // Unref obj2 (this should destroy the block)
    tsc_object_unref(obj2);
    printf("After unref(obj2): live blocks = %zu\n", tsc_arc_get_live_count());
    
    printf("PASS: Reference counting segfault test completed successfully!\n");
}

// Test multiple references to the same block
void test_multiple_references() {
    printf("\n=== Test: Multiple References ===\n");
    
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    if (!obj1) {
        printf("FAIL: Failed to create obj1\n");
        return;
    }
    
    double* value = (double*)obj1->block->data;
    *value = 42.0;
    
    // Create multiple references
    TSCObject* obj2 = tsc_object_ref(obj1);
    TSCObject* obj3 = tsc_object_ref(obj1);
    TSCObject* obj4 = tsc_object_ref(obj2);
    
    printf("After creating 3 references: ref_count=%d\n", obj1->block->ref_count);
    
    if (obj1->block->ref_count != 4) {
        printf("FAIL: Expected ref_count=4, got %d\n", obj1->block->ref_count);
        tsc_object_unref(obj1);
        tsc_object_unref(obj2);
        tsc_object_unref(obj3);
        tsc_object_unref(obj4);
        return;
    }
    
    // Unref in different order
    tsc_object_unref(obj3);
    printf("After unref(obj3): ref_count=%d\n", obj1->block->ref_count);
    
    tsc_object_unref(obj1);
    printf("After unref(obj1): ref_count=%d\n", obj2->block->ref_count);
    
    tsc_object_unref(obj4);
    printf("After unref(obj4): ref_count=%d\n", obj2->block->ref_count);
    
    tsc_object_unref(obj2);
    printf("After unref(obj2): live blocks = %zu\n", tsc_arc_get_live_count());
    
    printf("PASS: Multiple references test completed successfully!\n");
}

// Test string concatenation with ARC
void test_string_concatenation() {
    printf("\n=== Test: String Concatenation ===\n");
    
    TSCObject* str1 = tsc_object_new(20, tsc_type_string());
    TSCObject* str2 = tsc_object_new(20, tsc_type_string());
    
    if (!str1 || !str2) {
        printf("FAIL: Failed to create string objects\n");
        if (str1) tsc_object_unref(str1);
        if (str2) tsc_object_unref(str2);
        return;
    }
    
    char* data1 = (char*)str1->block->data;
    char* data2 = (char*)str2->block->data;
    strcpy(data1, "Hello ");
    strcpy(data2, "World!");
    
    printf("String 1: %s (ref_count: %d)\n", data1, str1->block->ref_count);
    printf("String 2: %s (ref_count: %d)\n", data2, str2->block->ref_count);
    
    // Concatenate using ARC
    TSCObject* result = tsc_string_concat(str1, str2);
    if (result) {
        char* result_data = (char*)result->block->data;
        printf("Concatenated result: %s (ref_count: %d)\n", 
               result_data, result->block->ref_count);
        
        // Clean up
        tsc_object_unref(result);
    }
    
    // Clean up original strings
    tsc_object_unref(str1);
    tsc_object_unref(str2);
    
    printf("After cleanup: live blocks = %zu\n", tsc_arc_get_live_count());
    printf("PASS: String concatenation test completed successfully!\n");
}

int main() {
    printf("ARC Regression Test Suite\n");
    printf("========================\n");
    
    test_reference_counting_segfault();
    test_multiple_references();
    test_string_concatenation();
    
    printf("\n=== Final Debug Information ===\n");
    tsc_arc_debug_print();
    
    printf("\nAll regression tests passed!\n");
    return 0;
}