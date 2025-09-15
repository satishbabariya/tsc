// ARC Reference Lifecycle Demo
#include <stdio.h>
#include <stdlib.h>
#include "runtime/arc/arc_runtime.h"

// Simple object structure
typedef struct {
    int value;
} MyObject;

// Destructor for our object
void my_object_destructor(void* obj) {
    MyObject* my_obj = (MyObject*)obj;
    printf("🔴 DESTRUCTOR CALLED: Object with value %d is being deallocated\n", my_obj->value);
    // In a real implementation, we'd free any resources here
}

int main() {
    printf("=== ARC Strong Reference Lifecycle Demo ===\n\n");
    
    // Step 1: Create object with ARC
    printf("Step 1: Creating object with ARC allocation\n");
    MyObject* obj = (MyObject*)__tsc_alloc(sizeof(MyObject), my_object_destructor, NULL);
    obj->value = 42;
    printf("✅ Object created at %p with value %d\n", obj, obj->value);
    printf("📊 Initial ref count: 1\n\n");
    
    // Step 2: Create strong reference (copy)
    printf("Step 2: Creating strong reference (copy)\n");
    __tsc_retain(obj);  // Increment reference count
    printf("✅ Strong reference created\n");
    printf("📊 Ref count after retain: 2\n\n");
    
    // Step 3: Create another strong reference
    printf("Step 3: Creating another strong reference\n");
    __tsc_retain(obj);  // Increment reference count again
    printf("✅ Another strong reference created\n");
    printf("📊 Ref count after second retain: 3\n\n");
    
    // Step 4: Release first strong reference
    printf("Step 4: Releasing first strong reference\n");
    __tsc_release(obj);  // Decrement reference count
    printf("✅ First strong reference released\n");
    printf("📊 Ref count after first release: 2\n\n");
    
    // Step 5: Release second strong reference
    printf("Step 5: Releasing second strong reference\n");
    __tsc_release(obj);  // Decrement reference count
    printf("✅ Second strong reference released\n");
    printf("📊 Ref count after second release: 1\n\n");
    
    // Step 6: Release original reference - OBJECT DEALLOCATED!
    printf("Step 6: Releasing original reference\n");
    __tsc_release(obj);  // Decrement to 0 - triggers deallocation
    printf("✅ Original reference released\n");
    printf("📊 Ref count after final release: 0\n");
    printf("🔴 Object should be deallocated now!\n\n");
    
    printf("=== Demo Complete ===\n");
    return 0;
}