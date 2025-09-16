// Exception handling runtime for TSC
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <setjmp.h>
#include "arc/arc_runtime.h"

// Simple exception type
typedef struct {
    int32_t type;  // Exception type identifier
    void* data;     // Exception data
} Exception;

// Global exception state
static Exception* current_exception = NULL;

// Exception handling context stack
typedef struct {
    jmp_buf env;
    Exception* exception;
    int is_active;
} exception_context_t;

static exception_context_t* exception_stack = NULL;
static int exception_stack_size = 0;
static int exception_stack_capacity = 0;

// Runtime functions called by generated LLVM code
void __throw_exception(int64_t exception_value) {
    // Convert the exception value to our exception structure using ARC allocation
    Exception* ex = (Exception*)__tsc_alloc(sizeof(Exception), NULL, NULL);
    ex->type = 1;  // Generic exception type
    ex->data = (void*)(intptr_t)exception_value;
    
    current_exception = ex;
    
    // Enhanced exception handling with setjmp/longjmp
    if (exception_stack_size > 0) {
        // Find the most recent active exception context
        for (int i = exception_stack_size - 1; i >= 0; i--) {
            if (exception_stack[i].is_active) {
                exception_stack[i].exception = ex;
                longjmp(exception_stack[i].env, 1);
            }
        }
    }
    
    // No active exception handler found, print and exit
    printf("Unhandled exception: %ld\n", exception_value);
    exit(1);
}

void __rethrow_exception() {
    if (current_exception) {
        printf("Exception re-thrown: %ld\n", (int64_t)(intptr_t)current_exception->data);
        exit(1);
    } else {
        printf("No exception to re-throw\n");
        exit(1);
    }
}

// Helper function to check if an exception is currently active
int __has_exception() {
    return current_exception != NULL;
}

// Helper function to get the current exception
int64_t __get_exception() {
    if (current_exception) {
        return (int64_t)(intptr_t)current_exception->data;
    }
    return 0;
}

// Exception context management functions
void __push_exception_context(jmp_buf* env) {
    if (exception_stack_size >= exception_stack_capacity) {
        exception_stack_capacity = exception_stack_capacity == 0 ? 16 : exception_stack_capacity * 2;
        exception_stack = realloc(exception_stack, exception_stack_capacity * sizeof(exception_context_t));
    }
    
    exception_context_t* ctx = &exception_stack[exception_stack_size++];
    memcpy(&ctx->env, env, sizeof(jmp_buf));
    ctx->exception = NULL;
    ctx->is_active = 1;
}

void __pop_exception_context() {
    if (exception_stack_size > 0) {
        exception_stack_size--;
    }
}

Exception* __get_current_exception() {
    return current_exception;
}

void __clear_exception() {
    if (current_exception) {
        __tsc_release(current_exception);
        current_exception = NULL;
    }
}