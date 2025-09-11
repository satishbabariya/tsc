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

// ARC-based string concatenation is now implemented in runtime_arc.c

// Simple runtime exception handling
void __throw_exception(void* exception_value) {
    printf("Exception thrown: %p\n", exception_value);
    exit(1); // Simple termination for now
}

void __rethrow_exception(void) {
    printf("Re-throwing exception\n");
    exit(1); // Simple termination for now
}