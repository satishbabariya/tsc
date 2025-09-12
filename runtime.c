#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// String concatenation function
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

// Simple runtime exception handling
void __throw_exception(void* exception_value) {
    printf("Exception thrown: %p\n", exception_value);
    exit(1); // Simple termination for now
}

void __rethrow_exception(void) {
    printf("Re-throwing exception\n");
    exit(1); // Simple termination for now
}

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

// Type conversion functions for template literals

// Convert number to string
// Signature: char* number_to_string(double value)
// Returns: A newly allocated string representation of the number
char* number_to_string(double value) {
    // Allocate buffer for the string representation
    // Use snprintf to safely convert double to string
    char* buffer = (char*)malloc(32); // 32 chars should be enough for most doubles
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed in number_to_string\n");
        exit(1);
    }
    
    // Convert double to string
    snprintf(buffer, 32, "%.6g", value); // Use %g for compact representation
    
    return buffer;
}

// Convert boolean to string
// Signature: char* boolean_to_string(bool value)
// Returns: A newly allocated string representation of the boolean
char* boolean_to_string(bool value) {
    char* result = (char*)malloc(value ? 5 : 6); // "true" or "false"
    if (!result) {
        fprintf(stderr, "Memory allocation failed in boolean_to_string\n");
        exit(1);
    }
    
    strcpy(result, value ? "true" : "false");
    return result;
}

// Convert object to string (placeholder implementation)
// Signature: char* object_to_string(void* obj)
// Returns: A newly allocated string representation of the object
char* object_to_string(void* obj) {
    if (!obj) {
        char* result = (char*)malloc(5); // "null"
        if (!result) {
            fprintf(stderr, "Memory allocation failed in object_to_string\n");
            exit(1);
        }
        strcpy(result, "null");
        return result;
    }
    
    // For now, return a placeholder string with the object address
    char* result = (char*)malloc(32);
    if (!result) {
        fprintf(stderr, "Memory allocation failed in object_to_string\n");
        exit(1);
    }
    
    snprintf(result, 32, "[object %p]", obj);
    return result;
}

// Global constants for JavaScript built-ins
double Infinity = 1.0 / 0.0;  // Positive infinity
double NaN = 0.0 / 0.0;        // Not a Number