// Array operations runtime for TSC
#include <stdio.h>
#include <stdlib.h>
#include "arc/arc_runtime.h"

// Array length function
// Signature: int array_length(void* array_ptr)
// Returns: The length of the array (first i32 field)
int array_length(void* array_ptr) {
    if (!array_ptr) {
        fprintf(stderr, "Error: array_length called with null pointer\n");
        exit(3); // Exit code 3 for null pointer
    }
    
    // The array structure is { i32 length, [N x elementType] data }
    // We need to read the first i32 field (length)
    int* length_ptr = (int*)array_ptr;
    int length = *length_ptr;
    
    printf("DEBUG: array_length called with ptr=%p, length=%d\n", array_ptr, length);
    return length;
}

// Array push function
// Signature: void arrayPush(void* array_ptr, void* item)
// Adds an item to the end of the array
void arrayPush(void* array_ptr, void* item) {
    printf("DEBUG: arrayPush ENTRY - array_ptr=%p, item=%p\n", array_ptr, item);
    if (!array_ptr) {
        fprintf(stderr, "Error: arrayPush called with null array pointer\n");
        exit(3);
    }
    printf("DEBUG: arrayPush - array_ptr is not null, proceeding...\n");
    
    // The new array structure is { i32 length, ptr data }
    // We need to access the length field correctly
    struct {
        int length;
        void* data_ptr;
    }* array_struct = (void*)array_ptr;
    
    printf("DEBUG: arrayPush - array_struct=%p, length_ptr=%p, data_ptr=%p\n", 
           array_struct, &array_struct->length, array_struct->data_ptr);
    
    int current_length = array_struct->length;
    
    printf("DEBUG: arrayPush called with array=%p, item=%p, current_length=%d\n", 
           array_ptr, item, current_length);
    
    // For now, this is a placeholder implementation
    // In a full implementation, we would:
    // 1. Check if the array has space for another item
    // 2. Store the item at the appropriate position using data_ptr
    // 3. Increment the length
    // For now, just increment the length as a proof of concept
    array_struct->length = current_length + 1;
    
    printf("DEBUG: arrayPush updated length to %d\n", array_struct->length);
}

// Array pop function
// Signature: void* arrayPop(void* array_ptr)
// Removes and returns the last item from the array
void* arrayPop(void* array_ptr) {
    if (!array_ptr) {
        fprintf(stderr, "Error: arrayPop called with null array pointer\n");
        exit(3);
    }
    
    // The array structure is { i32 length, [N x elementType] data }
    int* length_ptr = (int*)array_ptr;
    int current_length = *length_ptr;
    
    printf("DEBUG: arrayPop called with array=%p, current_length=%d\n", array_ptr, current_length);
    
    if (current_length <= 0) {
        printf("DEBUG: arrayPop called on empty array, returning null\n");
        return NULL;
    }
    
    // For now, this is a placeholder implementation
    // In a full implementation, we would:
    // 1. Get the item at the last position
    // 2. Decrement the length
    // 3. Return the item
    // For now, just decrement the length and return a placeholder
    *length_ptr = current_length - 1;
    
    printf("DEBUG: arrayPop updated length to %d\n", *length_ptr);
    
    // Return a placeholder value (in a real implementation, this would be the actual item)
    return (void*)0x12345678; // Placeholder address
}

// Wrapper functions for mangled arrayPush functions
// These provide the mangled names that the linker expects
#ifdef __APPLE__
// On macOS, create wrapper functions with the expected names
void arrayPush_1(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_2(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_3(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_4(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_5(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_6(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_7(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_8(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_9(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_10(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
void arrayPush_11(void* array_ptr, void* element) { arrayPush(array_ptr, element); }
#else
// On Linux, use assembly directives
__asm__(".weak arrayPush.1");
__asm__(".set arrayPush.1, arrayPush");
__asm__(".weak arrayPush.2");
__asm__(".set arrayPush.2, arrayPush");
__asm__(".weak arrayPush.3");
__asm__(".set arrayPush.3, arrayPush");
__asm__(".weak arrayPush.4");
__asm__(".set arrayPush.4, arrayPush");
__asm__(".weak arrayPush.5");
__asm__(".set arrayPush.5, arrayPush");
__asm__(".weak arrayPush.6");
__asm__(".set arrayPush.6, arrayPush");
__asm__(".weak arrayPush.7");
__asm__(".set arrayPush.7, arrayPush");
__asm__(".weak arrayPush.8");
__asm__(".set arrayPush.8, arrayPush");
__asm__(".weak arrayPush.9");
__asm__(".set arrayPush.9, arrayPush");
__asm__(".weak arrayPush.10");
__asm__(".set arrayPush.10, arrayPush");
__asm__(".weak arrayPush.11");
__asm__(".set arrayPush.11, arrayPush");
#endif