// TSC Runtime Library Header
#ifndef TSC_RUNTIME_H
#define TSC_RUNTIME_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// String operations
char* string_concat(char* str1, char* str2);
char* number_to_string(double value);
char* boolean_to_string(bool value);
char* object_to_string(void* obj);
char* stringToString(char* str);

// Array operations
int array_length(void* array_ptr);
void arrayPush(void* array_ptr, void* item);
void* arrayPop(void* array_ptr);

// Exception handling
void __throw_exception(int64_t exception_value);
void __rethrow_exception(void);
int __has_exception(void);
int64_t __get_exception(void);
void __clear_exception(void);

// Array bounds checking
void panic_bounds_error(int32_t index, int32_t length);

// Internal print function
void _print(void* first_arg, ...);

// Console operations
void console_log(void* arg);

// Memory audit and testing functions
#include "memory_audit.h"

// Memory operations are handled by ARC runtime functions
// See arc/arc_runtime.h for __tsc_alloc, __tsc_retain, __tsc_release, etc.

// Global constants
extern double Infinity;
extern double NaN;

#endif // TSC_RUNTIME_H