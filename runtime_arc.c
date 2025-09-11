#include "runtime_arc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Global tracking for debugging
static TSCARCBlock** live_blocks = NULL;
static size_t live_count = 0;
static size_t live_capacity = 0;
static uint64_t next_allocation_id = 1;

// Default destructors
static void tsc_default_destructor(void* data) {
    if (data) {
        free(data);
    }
}

static void tsc_string_destructor(void* data) {
    if (data) {
        free(data);
    }
}

static void tsc_object_destructor(void* data) {
    if (data) {
        // For objects, we need to recursively clean up properties
        // This is a simplified version - in a full implementation,
        // we'd traverse the object's properties and decrement their ref counts
        free(data);
    }
}

// Type definitions
static TypeInfo number_type = {
    .name = "number",
    .size = sizeof(double),
    .destructor = tsc_default_destructor,
    .is_primitive = true
};

static TypeInfo string_type = {
    .name = "string", 
    .size = sizeof(char*),
    .destructor = tsc_string_destructor,
    .is_primitive = false
};

static TypeInfo boolean_type = {
    .name = "boolean",
    .size = sizeof(bool),
    .destructor = tsc_default_destructor,
    .is_primitive = true
};

static TypeInfo object_type = {
    .name = "object",
    .size = 0, // Variable size
    .destructor = tsc_object_destructor,
    .is_primitive = false
};

static TypeInfo array_type = {
    .name = "array",
    .size = 0, // Variable size
    .destructor = tsc_object_destructor, // Arrays are objects
    .is_primitive = false
};

// ARC implementation
TSCARCBlock* tsc_arc_block_new(size_t size, TypeInfo* type_info) {
    if (!type_info) {
        tsc_panic("TypeInfo cannot be NULL");
        return NULL;
    }
    
    TSCARCBlock* block = malloc(sizeof(TSCARCBlock));
    if (!block) {
        tsc_panic("Failed to allocate TSCARCBlock");
        return NULL;
    }
    
    // Allocate data
    size_t data_size = (size > 0) ? size : type_info->size;
    void* data = malloc(data_size);
    if (!data) {
        free(block);
        tsc_panic("Failed to allocate data for TSCARCBlock");
        return NULL;
    }
    
    // Initialize block
    block->ref_count = 1;
    block->data = data;
    block->type_info = type_info;
    block->destructor = type_info->destructor;
    block->is_owned = true;
    block->allocation_id = next_allocation_id++;
    
    // Add to live blocks for tracking
    if (live_count >= live_capacity) {
        live_capacity = live_capacity ? live_capacity * 2 : 16;
        live_blocks = realloc(live_blocks, live_capacity * sizeof(TSCARCBlock*));
        if (!live_blocks) {
            free(data);
            free(block);
            tsc_panic("Failed to allocate live_blocks array");
            return NULL;
        }
    }
    
    live_blocks[live_count++] = block;
    
    return block;
}

void tsc_arc_increment(TSCARCBlock* block) {
    if (block && block->ref_count > 0) {
        block->ref_count++;
    }
}

void tsc_arc_decrement(TSCARCBlock* block) {
    if (!block || block->ref_count <= 0) return;
    
    block->ref_count--;
    
    if (block->ref_count <= 0) {
        tsc_arc_destroy(block);
    }
}

void tsc_arc_destroy(TSCARCBlock* block) {
    if (!block) return;
    
    // Call destructor
    if (block->destructor && block->data) {
        block->destructor(block->data);
    }
    
    // Remove from live blocks
    for (size_t i = 0; i < live_count; i++) {
        if (live_blocks[i] == block) {
            live_blocks[i] = live_blocks[--live_count];
            break;
        }
    }
    
    // Free the block itself
    free(block);
}

// Object management
TSCObject* tsc_object_new(size_t size, TypeInfo* type_info) {
    TSCARCBlock* block = tsc_arc_block_new(size, type_info);
    if (!block) return NULL;
    
    TSCObject* obj = malloc(sizeof(TSCObject));
    if (!obj) {
        tsc_arc_destroy(block);
        tsc_panic("Failed to allocate TSCObject");
        return NULL;
    }
    
    obj->block = block;
    return obj;
}

TSCObject* tsc_object_ref(TSCObject* obj) {
    if (obj && obj->block) {
        tsc_arc_increment(obj->block);
    }
    return obj;
}

void tsc_object_unref(TSCObject* obj) {
    if (obj) {
        if (obj->block) {
            tsc_arc_decrement(obj->block);
            obj->block = NULL; // Prevent double-free
        }
        free(obj);
    }
}

void tsc_object_destroy(TSCObject* obj) {
    if (obj) {
        tsc_arc_destroy(obj->block);
        free(obj);
    }
}

// Type system getters
TypeInfo* tsc_type_number(void) {
    return &number_type;
}

TypeInfo* tsc_type_string(void) {
    return &string_type;
}

TypeInfo* tsc_type_boolean(void) {
    return &boolean_type;
}

TypeInfo* tsc_type_object(void) {
    return &object_type;
}

TypeInfo* tsc_type_array(void) {
    return &array_type;
}

// Debugging and utilities
void tsc_arc_debug_print(void) {
    printf("=== ARC Debug Information ===\n");
    printf("Live blocks: %zu\n", live_count);
    printf("Allocation ID counter: %llu\n", (unsigned long long)next_allocation_id);
    
    for (size_t i = 0; i < live_count; i++) {
        TSCARCBlock* block = live_blocks[i];
        printf("Block %llu: type=%s, ref_count=%d, data=%p\n", 
               (unsigned long long)block->allocation_id,
               block->type_info->name,
               block->ref_count,
               block->data);
    }
    printf("=============================\n");
}

size_t tsc_arc_get_live_count(void) {
    return live_count;
}

void tsc_arc_cleanup_all(void) {
    // Clean up all remaining blocks
    for (size_t i = 0; i < live_count; i++) {
        TSCARCBlock* block = live_blocks[i];
        if (block->destructor && block->data) {
            block->destructor(block->data);
        }
        free(block);
    }
    
    live_count = 0;
    free(live_blocks);
    live_blocks = NULL;
    live_capacity = 0;
}

// Error handling
void tsc_panic(const char* message) {
    fprintf(stderr, "TSC Runtime Error: %s\n", message);
    tsc_arc_debug_print();
    exit(1);
}