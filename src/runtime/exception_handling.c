// Exception handling runtime for TSC
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <setjmp.h>
#include <string.h>
#include "arc/arc_runtime.h"

// Exception type identifiers
#define EXCEPTION_TYPE_GENERIC 1
#define EXCEPTION_TYPE_RUNTIME_ERROR 2
#define EXCEPTION_TYPE_TYPE_ERROR 3
#define EXCEPTION_TYPE_VALUE_ERROR 4
#define EXCEPTION_TYPE_REFERENCE_ERROR 5

// Exception structure
typedef struct {
    int32_t type; // Exception type identifier
    void *data; // Exception data
    char *message; // Exception message
    char *stack_trace; // Stack trace (for debugging)
} Exception;

// Exception handler context
typedef struct {
    jmp_buf jump_buffer; // setjmp/longjmp buffer
    Exception *exception; // Current exception
    struct ExceptionHandler *parent; // Parent handler for nested try-catch
} ExceptionHandler;

// Global exception state
static Exception *current_exception = NULL;
static ExceptionHandler *current_handler = NULL;
static ExceptionHandler *handler_stack = NULL;

// Helper function to create exception with message
static Exception *create_exception(int32_t type, int64_t value, const char *message) {
    Exception *ex = (Exception *) __tsc_alloc(sizeof(Exception), NULL, NULL);
    ex->type = type;
    ex->data = (void *) (intptr_t) value;

    // Allocate and copy message
    if (message) {
        size_t msg_len = strlen(message) + 1;
        ex->message = (char *) __tsc_alloc(msg_len, NULL, NULL);
        strcpy(ex->message, message);
    } else {
        ex->message = NULL;
    }

    ex->stack_trace = NULL; // TODO: Implement stack trace generation

    return ex;
}

// Helper function to free exception
static void free_exception(Exception *ex) {
    if (ex) {
        if (ex->message) {
            __tsc_release(ex->message);
        }
        if (ex->stack_trace) {
            __tsc_release(ex->stack_trace);
        }
        __tsc_release(ex);
    }
}

// Runtime functions called by generated LLVM code
void __throw_exception(int64_t exception_value) {
    // Create exception with generic message
    Exception *ex = create_exception(EXCEPTION_TYPE_GENERIC, exception_value, "Generic exception");

    // Set as current exception
    current_exception = ex;

    // Find the most recent exception handler
    ExceptionHandler *handler = current_handler;
    while (handler && !handler->exception) {
        handler = handler->parent;
    }

    if (handler) {
        // Set exception in handler and longjmp to catch block
        handler->exception = ex;
        longjmp(handler->jump_buffer, 1);
    } else {
        // No handler found, print error and exit
        printf("Unhandled exception: %ld\n", exception_value);
        if (ex->message) {
            printf("Message: %s\n", ex->message);
        }
        free_exception(ex);
        exit(1);
    }
}

void __rethrow_exception() {
    if (current_exception) {
        // Find the next exception handler up the stack
        ExceptionHandler *handler = current_handler;
        while (handler && handler->exception) {
            handler = handler->parent;
        }

        if (handler) {
            // Set exception in handler and longjmp to catch block
            handler->exception = current_exception;
            longjmp(handler->jump_buffer, 1);
        } else {
            // No handler found, print error and exit
            printf("Unhandled re-thrown exception: %ld\n", (int64_t) (intptr_t) current_exception->data);
            if (current_exception->message) {
                printf("Message: %s\n", current_exception->message);
            }
            free_exception(current_exception);
            exit(1);
        }
    } else {
        printf("No exception to re-throw\n");
        exit(1);
    }
}

// Exception handler management functions
void __setup_exception_handler(ExceptionHandler *handler) {
    handler->exception = NULL;
    handler->parent = current_handler;
    current_handler = handler;
}

void __cleanup_exception_handler(ExceptionHandler *handler) {
    if (current_handler == handler) {
        current_handler = handler->parent;
    }

    // Clean up any exception in this handler
    if (handler->exception) {
        free_exception(handler->exception);
        handler->exception = NULL;
    }
}

int __try_exception_handler(ExceptionHandler *handler) {
    return setjmp(handler->jump_buffer);
}

// Helper function to check if an exception is currently active
int __has_exception() {
    return current_exception != NULL;
}

// Helper function to get the current exception
int64_t __get_exception() {
    if (current_exception) {
        return (int64_t) (intptr_t) current_exception->data;
    }
    return 0;
}

// Helper function to get exception type
int32_t __get_exception_type() {
    if (current_exception) {
        return current_exception->type;
    }
    return 0;
}

// Helper function to get exception message
const char *__get_exception_message() {
    if (current_exception && current_exception->message) {
        return current_exception->message;
    }
    return "Unknown exception";
}

// Helper function to clear the current exception
void __clear_exception() {
    if (current_exception) {
        free_exception(current_exception);
        current_exception = NULL;
    }
}

// Specialized exception throwing functions
void __throw_runtime_error(const char *message) {
    Exception *ex = create_exception(EXCEPTION_TYPE_RUNTIME_ERROR, 0, message);
    current_exception = ex;

    ExceptionHandler *handler = current_handler;
    while (handler && !handler->exception) {
        handler = handler->parent;
    }

    if (handler) {
        handler->exception = ex;
        longjmp(handler->jump_buffer, 1);
    } else {
        printf("Unhandled runtime error: %s\n", message);
        free_exception(ex);
        exit(1);
    }
}

void __throw_type_error(const char *message) {
    Exception *ex = create_exception(EXCEPTION_TYPE_TYPE_ERROR, 0, message);
    current_exception = ex;

    ExceptionHandler *handler = current_handler;
    while (handler && !handler->exception) {
        handler = handler->parent;
    }

    if (handler) {
        handler->exception = ex;
        longjmp(handler->jump_buffer, 1);
    } else {
        printf("Unhandled type error: %s\n", message);
        free_exception(ex);
        exit(1);
    }
}

void __throw_value_error(const char *message) {
    Exception *ex = create_exception(EXCEPTION_TYPE_VALUE_ERROR, 0, message);
    current_exception = ex;

    ExceptionHandler *handler = current_handler;
    while (handler && !handler->exception) {
        handler = handler->parent;
    }

    if (handler) {
        handler->exception = ex;
        longjmp(handler->jump_buffer, 1);
    } else {
        printf("Unhandled value error: %s\n", message);
        free_exception(ex);
        exit(1);
    }
}
