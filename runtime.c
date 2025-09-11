#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// String concatenation function
char* string_concat(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = malloc(len1 + len2 + 1);
    strcpy(result, str1);
    strcat(result, str2);
    return result;
}

// Console logging functions
void console_log(const char* message) {
    printf("%s\n", message);
}

void console_error(const char* message) {
    fprintf(stderr, "ERROR: %s\n", message);
}

// Math functions
double math_abs(double x) {
    return fabs(x);
}

double math_sqrt(double x) {
    return sqrt(x);
}

// String functions
int string_length(const char* str) {
    return strlen(str);
}

// Array functions
int array_length(void* array) {
    // This is a simplified implementation
    // In a real implementation, we'd need to track array metadata
    return 0;
}

// Exception handling (simplified)
void __throw_exception(const char* message) {
    fprintf(stderr, "Exception: %s\n", message);
    exit(1);
}

void __rethrow_exception() {
    fprintf(stderr, "Rethrowing exception\n");
    exit(1);
}

// Runtime function for number to string conversion
char* number_to_string(double num) {
    char* result = (char*)malloc(32); // Enough space for most numbers
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed in number_to_string\n");
        exit(1);
    }
    snprintf(result, 32, "%.15g", num); // Use %g for compact representation
    return result;
}

// Runtime function for boolean to string conversion
char* boolean_to_string(int boolean) {
    if (boolean) {
        char* result = (char*)malloc(5); // "true" + null terminator
        if (result == NULL) {
            fprintf(stderr, "Memory allocation failed in boolean_to_string\n");
            exit(1);
        }
        strcpy(result, "true");
        return result;
    } else {
        char* result = (char*)malloc(6); // "false" + null terminator
        if (result == NULL) {
            fprintf(stderr, "Memory allocation failed in boolean_to_string\n");
            exit(1);
        }
        strcpy(result, "false");
        return result;
    }
}