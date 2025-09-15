#include "smart_pointers.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// UniquePtr Implementation
// ============================================================================

TSC_UniquePtr* tsc_unique_ptr_create(void* ptr, void (*destructor)(void*), void* type_info) {
    TSC_UniquePtr* self = (TSC_UniquePtr*)malloc(sizeof(TSC_UniquePtr));
    if (!self) return NULL;
    
    self->ptr_ = ptr;
    self->destructor_ = destructor;
    self->type_info_ = type_info;
    
    return self;
}

void tsc_unique_ptr_destroy(TSC_UniquePtr* self) {
    if (!self) return;
    
    if (self->ptr_ && self->destructor_) {
        self->destructor_(self->ptr_);
    }
    
    free(self);
}

void* tsc_unique_ptr_get(TSC_UniquePtr* self) {
    return self ? self->ptr_ : NULL;
}

void tsc_unique_ptr_reset(TSC_UniquePtr* self, void* new_ptr) {
    if (!self) return;
    
    // Destroy old object
    if (self->ptr_ && self->destructor_) {
        self->destructor_(self->ptr_);
    }
    
    self->ptr_ = new_ptr;
}

TSC_UniquePtr* tsc_unique_ptr_move(TSC_UniquePtr* self) {
    if (!self) return NULL;
    
    TSC_UniquePtr* moved = (TSC_UniquePtr*)malloc(sizeof(TSC_UniquePtr));
    if (!moved) return NULL;
    
    // Transfer ownership
    moved->ptr_ = self->ptr_;
    moved->destructor_ = self->destructor_;
    moved->type_info_ = self->type_info_;
    
    // Clear source
    self->ptr_ = NULL;
    self->destructor_ = NULL;
    self->type_info_ = NULL;
    
    return moved;
}

// ============================================================================
// SharedPtr Implementation
// ============================================================================

TSC_SharedPtr* tsc_shared_ptr_create(void* ptr, void (*destructor)(void*), void* type_info) {
    TSC_SharedPtr* self = (TSC_SharedPtr*)malloc(sizeof(TSC_SharedPtr));
    if (!self) return NULL;
    
    self->ptr_ = ptr;
    self->destructor_ = destructor;
    self->type_info_ = type_info;
    
    if (ptr) {
        // Allocate ARC header
        self->header_ = (ARC_ObjectHeader*)malloc(sizeof(ARC_ObjectHeader));
        if (!self->header_) {
            free(self);
            return NULL;
        }
        
        // Initialize header
        atomic_init(&self->header_->ref_count, 1);
        atomic_init(&self->header_->weak_count, 0);
        self->header_->destructor = destructor;
        self->header_->type_info = type_info;
        
        // Retain the object
        __tsc_retain(ptr);
    } else {
        self->header_ = NULL;
    }
    
    return self;
}

void tsc_shared_ptr_destroy(TSC_SharedPtr* self) {
    if (!self) return;
    
    if (self->header_) {
        // Release the object
        __tsc_release(self->ptr_);
        
        // Free header if no more references
        if (atomic_load(&self->header_->ref_count) == 0) {
            free(self->header_);
        }
    }
    
    free(self);
}

void* tsc_shared_ptr_get(TSC_SharedPtr* self) {
    return self ? self->ptr_ : NULL;
}

void tsc_shared_ptr_reset(TSC_SharedPtr* self, void* new_ptr) {
    if (!self) return;
    
    // Release old object
    if (self->header_) {
        __tsc_release(self->ptr_);
        
        // Free header if no more references
        if (atomic_load(&self->header_->ref_count) == 0) {
            free(self->header_);
            self->header_ = NULL;
        }
    }
    
    // Set new object
    self->ptr_ = new_ptr;
    self->destructor_ = NULL; // Will be set by caller
    self->type_info_ = NULL; // Will be set by caller
    
    if (new_ptr) {
        // Allocate new header
        self->header_ = (ARC_ObjectHeader*)malloc(sizeof(ARC_ObjectHeader));
        if (self->header_) {
            atomic_init(&self->header_->ref_count, 1);
            atomic_init(&self->header_->weak_count, 0);
            self->header_->destructor = NULL; // Will be set by caller
            self->header_->type_info = NULL; // Will be set by caller
            
            // Retain the new object
            __tsc_retain(new_ptr);
        }
    }
}

TSC_SharedPtr* tsc_shared_ptr_copy(TSC_SharedPtr* self) {
    if (!self || !self->header_) return NULL;
    
    TSC_SharedPtr* copy = (TSC_SharedPtr*)malloc(sizeof(TSC_SharedPtr));
    if (!copy) return NULL;
    
    copy->ptr_ = self->ptr_;
    copy->header_ = self->header_;
    copy->destructor_ = self->destructor_;
    copy->type_info_ = self->type_info_;
    
    // Increment reference count
    __tsc_retain(self->ptr_);
    
    return copy;
}

TSC_SharedPtr* tsc_shared_ptr_move(TSC_SharedPtr* self) {
    if (!self) return NULL;
    
    TSC_SharedPtr* moved = (TSC_SharedPtr*)malloc(sizeof(TSC_SharedPtr));
    if (!moved) return NULL;
    
    // Transfer ownership
    moved->ptr_ = self->ptr_;
    moved->header_ = self->header_;
    moved->destructor_ = self->destructor_;
    moved->type_info_ = self->type_info_;
    
    // Clear source
    self->ptr_ = NULL;
    self->header_ = NULL;
    self->destructor_ = NULL;
    self->type_info_ = NULL;
    
    return moved;
}

uint32_t tsc_shared_ptr_use_count(TSC_SharedPtr* self) {
    if (!self || !self->header_) return 0;
    return atomic_load(&self->header_->ref_count);
}

// ============================================================================
// WeakPtr Implementation
// ============================================================================

TSC_WeakPtr* tsc_weak_ptr_create(ARC_ObjectHeader* header, void* type_info) {
    TSC_WeakPtr* self = (TSC_WeakPtr*)malloc(sizeof(TSC_WeakPtr));
    if (!self) return NULL;
    
    self->header_ = header;
    self->type_info_ = type_info;
    
    if (header) {
        // Increment weak reference count
        atomic_fetch_add(&header->weak_count, 1);
    }
    
    return self;
}

void tsc_weak_ptr_destroy(TSC_WeakPtr* self) {
    if (!self) return;
    
    if (self->header_) {
        // Decrement weak reference count
        atomic_fetch_sub(&self->header_->weak_count, 1);
    }
    
    free(self);
}

TSC_SharedPtr* tsc_weak_ptr_lock(TSC_WeakPtr* self) {
    if (!self || !self->header_) return NULL;
    
    // Check if object is still alive
    if (atomic_load(&self->header_->ref_count) == 0) {
        return NULL;
    }
    
    // Create shared_ptr
    TSC_SharedPtr* shared = (TSC_SharedPtr*)malloc(sizeof(TSC_SharedPtr));
    if (!shared) return NULL;
    
    shared->ptr_ = (void*)((char*)self->header_ + sizeof(ARC_ObjectHeader));
    shared->header_ = self->header_;
    shared->destructor_ = self->header_->destructor;
    shared->type_info_ = self->header_->type_info;
    
    // Increment reference count
    __tsc_retain(shared->ptr_);
    
    return shared;
}

uint32_t tsc_weak_ptr_use_count(TSC_WeakPtr* self) {
    if (!self || !self->header_) return 0;
    return atomic_load(&self->header_->ref_count);
}

int tsc_weak_ptr_expired(TSC_WeakPtr* self) {
    if (!self || !self->header_) return 1;
    return atomic_load(&self->header_->ref_count) == 0;
}

// ============================================================================
// std::move Implementation
// ============================================================================

void* tsc_std_move(void* ptr) {
    // In C, we can't actually "move" a pointer, but we can return it
    // The actual move semantics will be handled by the compiler
    return ptr;
}

// ============================================================================
// Utility Functions
// ============================================================================

void* tsc_alloc_object(size_t size, void (*destructor)(void*), void* type_info) {
    return __tsc_alloc(size, destructor, type_info);
}

void tsc_dealloc_object(void* ptr) {
    __tsc_release(ptr);
}