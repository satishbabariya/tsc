#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("Simple Reference Test\n");
    printf("=====================\n");
    
    // Create obj1
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    if (!obj1) {
        printf("Failed to create obj1\n");
        return 1;
    }
    
    double* value = (double*)obj1->block->data;
    *value = 100.0;
    printf("Created obj1: %f (ref_count: %d, block: %p)\n", *value, obj1->block->ref_count, obj1->block);
    
    // Create obj2 reference
    TSCObject* obj2 = tsc_object_ref(obj1);
    if (!obj2) {
        printf("Failed to create obj2\n");
        return 1;
    }
    
    printf("After ref: obj1 ref_count=%d, obj2 ref_count=%d\n", 
           obj1->block->ref_count, obj2->block->ref_count);
    printf("obj1 block: %p, obj2 block: %p\n", obj1->block, obj2->block);
    
    // Store the block pointer before unref
    TSCARCBlock* shared_block = obj1->block;
    printf("Shared block: %p\n", shared_block);
    
    // Unref obj1
    printf("\n--- Unref obj1 ---\n");
    tsc_object_unref(obj1);
    printf("After unref(obj1): live blocks = %zu\n", tsc_arc_get_live_count());
    
    // Check if obj2 is still valid
    printf("\n--- Check obj2 ---\n");
    printf("obj2: %p\n", obj2);
    printf("obj2->block: %p\n", obj2->block);
    printf("shared_block: %p\n", shared_block);
    
    if (obj2->block == shared_block) {
        printf("obj2->block matches shared_block\n");
        printf("obj2 ref_count: %d\n", obj2->block->ref_count);
    } else {
        printf("ERROR: obj2->block does not match shared_block!\n");
        printf("This indicates memory corruption!\n");
        return 1;
    }
    
    // Unref obj2
    printf("\n--- Unref obj2 ---\n");
    tsc_object_unref(obj2);
    printf("After unref(obj2): live blocks = %zu\n", tsc_arc_get_live_count());
    
    printf("\nTest completed successfully!\n");
    return 0;
}