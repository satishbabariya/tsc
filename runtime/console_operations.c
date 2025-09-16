#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "arc/arc_runtime.h"

// Helper function to safely handle string arguments
static void safe_print_string(char* str) {
    if (!str) {
        printf("(null)");
        return;
    }
    
    // Print the string directly
    printf("%s", str);
}

// Helper function to validate format string safety
static bool validate_format_string(char* format, int arg_count) {
    if (!format) return false;
    
    int format_count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++; // Move to next character
            if (format[i] == '\0') {
                // Format string ends with '%', invalid
                return false;
            }
            if (format[i] != '%') {
                // This is a format specifier (not literal %%)
                format_count++;
            }
        }
    }
    
    // Check if we have enough arguments for the format specifiers
    return format_count <= arg_count;
}

// Internal print function for static compiler
// This is a variadic function that can accept multiple arguments
void _print(void* first_arg, ...) {
    // Handle null format string gracefully
    if (!first_arg) {
        putchar('\n');
        return;
    }
    
    char* format = (char*)first_arg; // Treat the first argument as the format string
    
    // Validate format string is not empty
    if (strlen(format) == 0) {
        putchar('\n');
        return;
    }
    
    va_list args;
    va_start(args, first_arg); // Initialize the variable argument list

    // Use vprintf to handle the format string and arguments
    vprintf(format, args);

    va_end(args); // Clean up the variable argument list
    putchar('\n'); // Add newline at the end, as per original behavior
}