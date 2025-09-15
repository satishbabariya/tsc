#include "arc_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Weak reference table entry
typedef struct WeakRefEntry {
    void* weak_ref;           // Pointer to weak reference
    void* target_obj;         // Pointer to target object
    struct WeakRefEntry* next; // Linked list for collision handling
} WeakRefEntry;

// Weak reference table (simplified hash table)
#define WEAK_REF_TABLE_SIZE 1024
static WeakRefEntry* weak_ref_table[WEAK_REF_TABLE_SIZE] = {0};

// Hash function for weak reference table
static size_t weak_ref_hash(void* weak_ref) {
    return ((size_t)weak_ref) % WEAK_REF_TABLE_SIZE;
}

// Register a weak reference
void __tsc_register_weak_ref(void* weak_ref, void* target_obj) {
    if (!weak_ref || !target_obj) return;
    
    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry* entry = malloc(sizeof(WeakRefEntry));
    
    if (!entry) {
        fprintf(stderr, "ERROR: Failed to allocate WeakRefEntry\n");
        return;
    }
    
    entry->weak_ref = weak_ref;
    entry->target_obj = target_obj;
    entry->next = weak_ref_table[hash];
    weak_ref_table[hash] = entry;
    
    #ifdef TSC_ARC_DEBUG
    printf("DEBUG: Registered weak reference %p -> %p\n", weak_ref, target_obj);
    #endif
}

// Unregister a weak reference
void __tsc_unregister_weak_ref(void* weak_ref) {
    if (!weak_ref) return;
    
    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry** entry_ptr = &weak_ref_table[hash];
    
    while (*entry_ptr) {
        if ((*entry_ptr)->weak_ref == weak_ref) {
            WeakRefEntry* to_free = *entry_ptr;
            *entry_ptr = (*entry_ptr)->next;
            free(to_free);
            
            #ifdef TSC_ARC_DEBUG
            printf("DEBUG: Unregistered weak reference %p\n", weak_ref);
            #endif
            return;
        }
        entry_ptr = &(*entry_ptr)->next;
    }
}

// Check if weak reference is still valid
bool __tsc_is_weak_ref_valid(void* weak_ref) {
    if (!weak_ref) return false;
    
    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry* entry = weak_ref_table[hash];
    
    while (entry) {
        if (entry->weak_ref == weak_ref) {
            // Check if target object is still alive
            ARC_ObjectHeader* header = __tsc_get_header(entry->target_obj);
            return atomic_load(&header->ref_count) > 0;
        }
        entry = entry->next;
    }
    
    return false;
}

// Get target object from weak reference (if still alive)
void* __tsc_weak_ref_get_target(void* weak_ref) {
    if (!weak_ref) return NULL;
    
    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry* entry = weak_ref_table[hash];
    
    while (entry) {
        if (entry->weak_ref == weak_ref) {
            // Check if target object is still alive
            ARC_ObjectHeader* header = __tsc_get_header(entry->target_obj);
            if (atomic_load(&header->ref_count) > 0) {
                return __tsc_retain(entry->target_obj);
            }
            return NULL;
        }
        entry = entry->next;
    }
    
    return NULL;
}

// Clean up weak references when object is deallocated
void __tsc_cleanup_weak_refs(void* target_obj) {
    if (!target_obj) return;
    
    // Iterate through all weak reference entries
    for (size_t i = 0; i < WEAK_REF_TABLE_SIZE; i++) {
        WeakRefEntry** entry_ptr = &weak_ref_table[i];
        
        while (*entry_ptr) {
            if ((*entry_ptr)->target_obj == target_obj) {
                WeakRefEntry* to_free = *entry_ptr;
                *entry_ptr = (*entry_ptr)->next;
                
                #ifdef TSC_ARC_DEBUG
                printf("DEBUG: Cleaned up weak reference %p -> %p\n", 
                       to_free->weak_ref, target_obj);
                #endif
                
                free(to_free);
            } else {
                entry_ptr = &(*entry_ptr)->next;
            }
        }
    }
}

// Dump weak reference table (for debugging)
void __tsc_dump_weak_ref_table(void) {
    printf("=== Weak Reference Table ===\n");
    
    size_t total_entries = 0;
    for (size_t i = 0; i < WEAK_REF_TABLE_SIZE; i++) {
        WeakRefEntry* entry = weak_ref_table[i];
        size_t bucket_count = 0;
        
        while (entry) {
            bucket_count++;
            total_entries++;
            entry = entry->next;
        }
        
        if (bucket_count > 0) {
            printf("Bucket %zu: %zu entries\n", i, bucket_count);
        }
    }
    
    printf("Total weak references: %zu\n", total_entries);
    printf("============================\n");
}