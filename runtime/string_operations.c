// String operations runtime for TSC
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "arc/arc_runtime.h"

// String concatenation function
// Signature: char* string_concat(char* str1, char* str2)
// Returns: A newly allocated string containing str1 + str2
char* string_concat(char* str1, char* str2) {
    if (!str1) str1 = "";
    if (!str2) str2 = "";
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t total_len = len1 + len2 + 1; // +1 for null terminator
    
    // Use regular malloc for now
    char* result = (char*)malloc(total_len);
    if (!result) {
        fprintf(stderr, "Memory allocation failed in string_concat\n");
        exit(1);
    }
    
    strcpy(result, str1);
    strcat(result, str2);
    
    return result;
}

// Convert number to string
// Signature: char* number_to_string(double value)
// Returns: A newly allocated string representation of the number
char* number_to_string(double value) {
    // Allocate buffer for the string representation using malloc
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

// Mangled versions of number_to_string for different type contexts
// These functions provide the same functionality as number_to_string but with different mangled names
char* number_to_string_1(double value) {
    return number_to_string(value);
}

char* number_to_string_2(double value) {
    return number_to_string(value);
}

char* number_to_string_3(double value) {
    return number_to_string(value);
}

char* number_to_string_4(double value) {
    return number_to_string(value);
}

char* number_to_string_5(double value) {
    return number_to_string(value);
}

// Convert pointer to string representation
char* pointer_to_string(void* ptr) {
    char* buffer = (char*)malloc(32); // Allocate buffer dynamically
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed in pointer_to_string\n");
        exit(1);
    }
    snprintf(buffer, 32, "%p", ptr);
    return buffer;
}

char* number_to_string_6(double value) {
    return number_to_string(value);
}

char* number_to_string_7(double value) {
    return number_to_string(value);
}

char* number_to_string_8(double value) {
    return number_to_string(value);
}

char* number_to_string_9(double value) {
    return number_to_string(value);
}

// Wrapper functions for mangled names with dots
// These create symbols with the names that the linker expects
#ifdef __APPLE__
// On macOS, create wrapper functions with the expected names
// Note: We can't use dots in function names, so these are alternative implementations
char* number_to_string_1_dot(double value) { return number_to_string_1(value); }
char* number_to_string_2_dot(double value) { return number_to_string_2(value); }
char* number_to_string_3_dot(double value) { return number_to_string_3(value); }
char* number_to_string_4_dot(double value) { return number_to_string_4(value); }
char* number_to_string_5_dot(double value) { return number_to_string_5(value); }
char* number_to_string_6_dot(double value) { return number_to_string_6(value); }
char* number_to_string_7_dot(double value) { return number_to_string_7(value); }
char* number_to_string_8_dot(double value) { return number_to_string_8(value); }
char* number_to_string_9_dot(double value) { return number_to_string_9(value); }
char* number_to_string_11_dot(double value) { return number_to_string_7(value); }
char* number_to_string_12_dot(double value) { return number_to_string_8(value); }
char* number_to_string_13_dot(double value) { return number_to_string_7(value); }
#else
// On Linux, use assembly directives
__asm__(".weak number_to_string.1");
__asm__(".set number_to_string.1, number_to_string_1");
__asm__(".weak number_to_string.2");
__asm__(".set number_to_string.2, number_to_string_2");
__asm__(".weak number_to_string.3");
__asm__(".set number_to_string.3, number_to_string_3");
__asm__(".weak number_to_string.4");
__asm__(".set number_to_string.4, number_to_string_4");
__asm__(".weak number_to_string.5");
__asm__(".set number_to_string.5, number_to_string_5");
__asm__(".weak number_to_string.6");
__asm__(".set number_to_string.6, number_to_string_6");
__asm__(".weak number_to_string.7");
__asm__(".set number_to_string.7, number_to_string_7");
__asm__(".weak number_to_string.8");
__asm__(".set number_to_string.8, number_to_string_8");
__asm__(".weak number_to_string.9");
__asm__(".set number_to_string.9, number_to_string_9");
__asm__(".weak number_to_string.11");
__asm__(".set number_to_string.11, number_to_string_7");
__asm__(".weak number_to_string.12");
__asm__(".set number_to_string.12, number_to_string_8");
__asm__(".weak number_to_string.13");
__asm__(".set number_to_string.13, number_to_string_7");
#endif