#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runtime_arc.h"

// String concatenation function with ARC
// Signature: char* string_concat(char* str1, char* str2)
// Returns: A newly allocated string containing str1 + str2
char* string_concat(char* str1, char* str2) {
    if (!str1) str1 = "";
    if (!str2) str2 = "";
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2 + 1; // +1 for null terminator
    
    char* result = (char*)malloc(total_len);
    if (!result) {
        fprintf(stderr, "Memory allocation failed in string_concat\n");
        exit(1);
    }
    
    strcpy(result, str1);
    strcat(result, str2);
    
    return result;
}

// ARC-based string concatenation
// Returns a TSCObject with reference counting
TSCObject* tsc_string_concat(TSCObject* str1, TSCObject* str2) {
    if (!str1 || !str2) {
        tsc_panic("Cannot concatenate NULL strings");
        return NULL;
    }
    
    // Get string data
    char* s1 = (char*)str1->block->data;
    char* s2 = (char*)str2->block->data;
    
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t total_len = len1 + len2 + 1;
    
    // Create new string object with ARC
    TSCObject* result = tsc_object_new(total_len, tsc_type_string());
    if (!result) {
        tsc_panic("Failed to create string object");
        return NULL;
    }
    
    char* result_data = (char*)result->block->data;
    strcpy(result_data, s1);
    strcat(result_data, s2);
    
    return result;
}

// Simple runtime exception handling
void __throw_exception(void* exception_value) {
    printf("Exception thrown: %p\n", exception_value);
    exit(1); // Simple termination for now
}

void __rethrow_exception(void) {
    printf("Re-throwing exception\n");
    exit(1); // Simple termination for now
}