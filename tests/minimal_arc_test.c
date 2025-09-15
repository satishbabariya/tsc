// Minimal ARC Test - Test basic ARC runtime functions
#include <stdio.h>
#include <stdlib.h>
#include "../runtime/arc/arc_runtime.h"

int main() {
    printf("=== Testing ARC Runtime Functions ===\n");
    
    // Test 1: Basic allocation and retain/release
    printf("Test 1: Basic ARC allocation\n");
    
    // Allocate an object
    void* obj = __tsc_alloc(16, NULL, NULL);
    if (obj == NULL) {
        printf("ERROR: Failed to allocate object\n");
        return 1;
    }
    
    printf("Object allocated at: %p\n", obj);
    
    // Test retain
    printf("Testing retain...\n");
    void* retained = __tsc_retain(obj);
    printf("Retain returned: %p\n", retained);
    
    // Test release
    printf("Testing release...\n");
    __tsc_release(obj);
    printf("Release completed\n");
    
    // Test final release (should deallocate)
    printf("Testing final release...\n");
    __tsc_release(obj);
    printf("Final release completed\n");
    
    printf("=== ARC Test Completed ===\n");
    return 0;
}