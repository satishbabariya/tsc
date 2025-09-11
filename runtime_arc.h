#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Forward declarations
typedef struct TypeInfo TypeInfo;
typedef struct TSCARCBlock TSCARCBlock;
typedef struct TSCObject TSCObject;

// Basic type information for ARC
typedef struct TypeInfo {
    const char* name;           // Type name (e.g., "number", "string", "object")
    size_t size;               // Size of the type in bytes
    void (*destructor)(void*); // Destructor function for this type
    bool is_primitive;         // Whether this is a primitive type
} TypeInfo;

// ARC memory block with reference counting
typedef struct TSCARCBlock {
    int ref_count;             // Reference count
    void* data;                // Actual data
    TypeInfo* type_info;       // Type information
    void (*destructor)(void*); // Custom destructor (overrides type_info->destructor)
    bool is_owned;             // Whether this block owns its data
    uint64_t allocation_id;    // For debugging and tracking
} TSCARCBlock;

// RAII wrapper for TypeScript objects
typedef struct TSCObject {
    TSCARCBlock* block;
} TSCObject;

// ARC function declarations
TSCARCBlock* tsc_arc_block_new(size_t size, TypeInfo* type_info);
void tsc_arc_increment(TSCARCBlock* block);
void tsc_arc_decrement(TSCARCBlock* block);
void tsc_arc_destroy(TSCARCBlock* block);

// Object management
TSCObject* tsc_object_new(size_t size, TypeInfo* type_info);
TSCObject* tsc_object_ref(TSCObject* obj);
void tsc_object_unref(TSCObject* obj);
void tsc_object_destroy(TSCObject* obj);

// Type system
TypeInfo* tsc_type_number(void);
TypeInfo* tsc_type_string(void);
TypeInfo* tsc_type_boolean(void);
TypeInfo* tsc_type_object(void);
TypeInfo* tsc_type_array(void);

// Memory management utilities
void tsc_arc_debug_print(void);
size_t tsc_arc_get_live_count(void);
void tsc_arc_cleanup_all(void);

// Error handling
void tsc_panic(const char* message);

// String operations with ARC
TSCObject* tsc_string_concat(TSCObject* str1, TSCObject* str2);