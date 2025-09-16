#pragma once

#include <stdint.h>
#include <setjmp.h>

#ifdef __cplusplus
extern "C" {
#endif

// Exception type identifiers
#define EXCEPTION_TYPE_GENERIC 1
#define EXCEPTION_TYPE_RUNTIME_ERROR 2
#define EXCEPTION_TYPE_TYPE_ERROR 3
#define EXCEPTION_TYPE_VALUE_ERROR 4
#define EXCEPTION_TYPE_REFERENCE_ERROR 5

// Exception structure
typedef struct {
    int32_t type;           // Exception type identifier
    void* data;             // Exception data
    char* message;          // Exception message
    char* stack_trace;      // Stack trace (for debugging)
} Exception;

// Exception handler context
typedef struct {
    jmp_buf jump_buffer;    // setjmp/longjmp buffer
    Exception* exception;    // Current exception
    struct ExceptionHandler* parent; // Parent handler for nested try-catch
} ExceptionHandler;

// Core exception functions
void __throw_exception(int64_t exception_value);
void __rethrow_exception();

// Exception handler management
void __setup_exception_handler(ExceptionHandler* handler);
void __cleanup_exception_handler(ExceptionHandler* handler);
int __try_exception_handler(ExceptionHandler* handler);

// Exception query functions
int __has_exception();
int64_t __get_exception();
int32_t __get_exception_type();
const char* __get_exception_message();
void __clear_exception();

// Specialized exception throwing functions
void __throw_runtime_error(const char* message);
void __throw_type_error(const char* message);
void __throw_value_error(const char* message);

#ifdef __cplusplus
}
#endif
