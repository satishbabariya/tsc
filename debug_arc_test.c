#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("Debug ARC Test\n");
    printf("==============\n");
    
    // Test reference counting with detailed debug output
    printf("\n--- Creating obj1 ---\n");
    TSCObject* obj1 = tsc_object_new(sizeof(double), tsc_type_number());
    if (obj1) {
        double* value = (double*)obj1->block->data;
        *value = 100.0;
        printf("Created obj1: %f (ref_count: %d, block: %p)\n", *value, obj1->block->ref_count, obj1->block);
        
        printf("\n--- Creating obj2 reference ---\n");
        TSCObject* obj2 = tsc_object_ref(obj1);
        printf("After ref: obj1 ref_count=%d, obj2 ref_count=%d\n", 
               obj1->block->ref_count, obj2->block->ref_count);
        printf("obj1 block: %p, obj2 block: %p\n", obj1->block, obj2->block);
        
        printf("\n--- Unref obj1 ---\n");
        printf("About to unref obj1 (block: %p, ref_count: %d)\n", obj1->block, obj1->block->ref_count);
        tsc_object_unref(obj1);
        printf("After unref(obj1): live blocks = %zu\n", tsc_arc_get_live_count());
        
        printf("\n--- Unref obj2 ---\n");
        printf("About to unref obj2 (block: %p)\n", obj2->block);
        if (obj2->block) {
            printf("obj2 ref_count: %d\n", obj2->block->ref_count);
        } else {
            printf("obj2->block is NULL!\n");
        }
        tsc_object_unref(obj2);
        printf("After unref(obj2): live blocks = %zu\n", tsc_arc_get_live_count());
    }
    
    printf("\nTest completed!\n");
    return 0;
}