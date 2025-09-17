#ifndef TSC_SMART_POINTERS_H
#define TSC_SMART_POINTERS_H

#include "arc_runtime.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct TSC_UniquePtr TSC_UniquePtr;
typedef struct TSC_SharedPtr TSC_SharedPtr;
typedef struct TSC_WeakPtr TSC_WeakPtr;

// UniquePtr structure
struct TSC_UniquePtr {
    void* ptr_;           // Raw pointer to managed object
    void (*destructor_)(void*); // Destructor function
    void* type_info_;     // Type information for debugging
};

// SharedPtr structure
struct TSC_SharedPtr {
    void* ptr_;           // Raw pointer to managed object
    ARC_ObjectHeader* header_; // Reference counting header
    void (*destructor_)(void*); // Destructor function
    void* type_info_;     // Type information for debugging
};

// WeakPtr structure
struct TSC_WeakPtr {
    ARC_ObjectHeader* header_; // Reference counting header
    void* type_info_;     // Type information for debugging
};

// UniquePtr functions
TSC_UniquePtr* tsc_unique_ptr_create(void* ptr, void (*destructor)(void*), void* type_info);
void tsc_unique_ptr_destroy(TSC_UniquePtr* self);
void* tsc_unique_ptr_get(TSC_UniquePtr* self);
void tsc_unique_ptr_reset(TSC_UniquePtr* self, void* new_ptr);
TSC_UniquePtr* tsc_unique_ptr_move(TSC_UniquePtr* self);

// SharedPtr functions
TSC_SharedPtr* tsc_shared_ptr_create(void* ptr, void (*destructor)(void*), void* type_info);
void tsc_shared_ptr_destroy(TSC_SharedPtr* self);
void* tsc_shared_ptr_get(TSC_SharedPtr* self);
void tsc_shared_ptr_reset(TSC_SharedPtr* self, void* new_ptr);
TSC_SharedPtr* tsc_shared_ptr_copy(TSC_SharedPtr* self);
TSC_SharedPtr* tsc_shared_ptr_move(TSC_SharedPtr* self);
uint32_t tsc_shared_ptr_use_count(TSC_SharedPtr* self);

// WeakPtr functions
TSC_WeakPtr* tsc_weak_ptr_create(ARC_ObjectHeader* header, void* type_info);
void tsc_weak_ptr_destroy(TSC_WeakPtr* self);
TSC_SharedPtr* tsc_weak_ptr_lock(TSC_WeakPtr* self);
uint32_t tsc_weak_ptr_use_count(TSC_WeakPtr* self);
int tsc_weak_ptr_expired(TSC_WeakPtr* self);

// std::move function
void* tsc_std_move(void* ptr);

// Utility functions
void* tsc_alloc_object(size_t size, void (*destructor)(void*), void* type_info);
void tsc_dealloc_object(void* ptr);

#ifdef __cplusplus
}
#endif

#endif // TSC_SMART_POINTERS_H