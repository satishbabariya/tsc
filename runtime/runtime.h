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

// Internal print function
void _print(void* first_arg, ...);

// Console operations
void console_log(void* value);
void console_log_bool(int value);
void console_log_string(char* value);

// Type conversion operations
int _toBool(double value);

// Memory operations are handled by ARC runtime functions
// See arc/arc_runtime.h for __tsc_alloc, __tsc_retain, __tsc_release, etc.

// Global constants
extern double Infinity;
extern double NaN;

#endif // TSC_RUNTIME_H