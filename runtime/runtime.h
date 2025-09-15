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

// Console operations
void console_log(void* first_arg, ...);

// Memory operations (ARC-compatible malloc wrappers)
void* malloc_1(size_t size);
void* malloc_2(size_t size);
void* malloc_3(size_t size);
void* malloc_4(size_t size);
void* malloc_5(size_t size);
void* malloc_6(size_t size);
void* malloc_7(size_t size);
void* malloc_8(size_t size);
void* malloc_9(size_t size);
void* malloc_10(size_t size);
void* malloc_11(size_t size);
void* malloc_12(size_t size);
void* malloc_13(size_t size);
void* malloc_14(size_t size);
void* malloc_15(size_t size);
void* malloc_16(size_t size);

// Global constants
extern double Infinity;
extern double NaN;

#endif // TSC_RUNTIME_H