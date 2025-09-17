#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "arc/arc_runtime.h"
#include "runtime.h"

// Helper function to safely handle string arguments with ARC
static void safe_print_string(char *str) {
    if (!str) {
        printf("(null)");
        return;
    }

    // Check if this is an ARC-managed string
    if (__tsc_is_arc_object(str)) {
        // Retain the string to ensure it's not deallocated during printing
        char *retained_str = (char *) __tsc_retain(str);
        printf("%s", retained_str);
        // Release the temporary retain
        __tsc_release(retained_str);
    } else {
        // Regular C string, print directly
        printf("%s", str);
    }
}

// Helper function to validate format string safety
static bool validate_format_string(char *format, int arg_count) {
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
void _print(void *first_arg, ...) {
    // Handle null format string gracefully
    if (!first_arg) {
        putchar('\n');
        return;
    }

    char *format = (char *) first_arg; // Treat the first argument as the format string

    // Validate format string is not empty
    if (strlen(format) == 0) {
        putchar('\n');
        return;
    }

    va_list args;
    va_start(args, first_arg); // Initialize the variable argument list

    // Count arguments for format string validation
    int arg_count = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            if (format[i] != '\0' && format[i] != '%') {
                arg_count++;
            }
        }
    }

    // Validate format string safety
    if (!validate_format_string(format, arg_count)) {
        // If format string is invalid, print it as literal text
        printf("Invalid format string: %s", format);
        va_end(args);
        putchar('\n');
        return;
    }

    // Iterate through the format string
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] != '%') {
            // Print non-format characters directly
            putchar(format[i]);
        } else {
            // Handle format specifiers
            i++; // Move to the character after '%'
            if (format[i] == '\0') {
                // Handle case where format string ends with '%'
                putchar('%');
                break;
            }
            switch (format[i]) {
                case 's': {
                    // String
                    char *str = va_arg(args, char*);
                    safe_print_string(str);
                    break;
                }
                case 'd': {
                    // Integer
                    int num = va_arg(args, int);
                    printf("%d", num);
                    break;
                }
                case 'f': {
                    // Float (double for va_arg)
                    double num = va_arg(args, double);
                    printf("%f", num);
                    break;
                }
                case 'c': {
                    // Character
                    int ch = va_arg(args, int); // char is promoted to int in va_arg
                    putchar(ch);
                    break;
                }
                case '%': {
                    // Literal '%'
                    putchar('%');
                    break;
                }
                default: {
                    // Handle unrecognized format specifiers by printing them as is
                    putchar('%');
                    putchar(format[i]);
                    break;
                }
            }
        }
    }

    va_end(args); // Clean up the variable argument list
    putchar('\n'); // Add newline at the end, as per original behavior
}

// Console.log function - simplified version that takes a single argument
// This function converts the argument to string and prints it
void console_log(void *arg) {
    if (!arg) {
        printf("undefined\n");
        return;
    }
    
    // For now, we'll use a simple approach - convert the argument to string
    // In a full implementation, we'd need to handle different types properly
    char *str = object_to_string(arg);
    if (str) {
        safe_print_string(str);
        putchar('\n');
        // Release the string if it's ARC-managed
        if (__tsc_is_arc_object(str)) {
            __tsc_release(str);
        }
    } else {
        printf("(null)\n");
    }
}
