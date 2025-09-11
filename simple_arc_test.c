#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    printf("Simple ARC Test\n");
    printf("===============\n");
    
    // Create a simple object
    TSCObject* obj = tsc_object_new(sizeof(double), tsc_type_number());
    if (obj) {
        double* value = (double*)obj->block->data;
        *value = 42.0;
        printf("Created object with value: %f\n", *value);
        printf("Reference count: %d\n", obj->block->ref_count);
        
        // Unref it
        tsc_object_unref(obj);
        printf("After unref: live blocks = %zu\n", tsc_arc_get_live_count());
    }
    
    printf("Test completed successfully!\n");
    return 0;
}