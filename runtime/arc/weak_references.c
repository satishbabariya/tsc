#include "arc_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Weak reference table entry
typedef struct WeakRefEntry {
    void *weak_ref; // Pointer to weak reference
    void *target_obj; // Pointer to target object
    struct WeakRefEntry *next; // Linked list for collision handling
} WeakRefEntry;

// Enhanced weak reference table with statistics
#define WEAK_REF_TABLE_SIZE 1024
static WeakRefEntry *weak_ref_table[WEAK_REF_TABLE_SIZE] = {0};

// Weak reference statistics
typedef struct {
    size_t total_registrations;
    size_t total_unregistrations;
    size_t total_valid_checks;
    size_t total_target_gets;
    size_t total_cleanups;
    size_t current_active_refs;
    size_t peak_active_refs;
} WeakRefStats;

static WeakRefStats g_weak_ref_stats = {0};

// Hash function for weak reference table
static size_t weak_ref_hash(void *weak_ref) {
    return ((size_t) weak_ref) % WEAK_REF_TABLE_SIZE;
}

// Register a weak reference
void __tsc_register_weak_ref(void *weak_ref, void *target_obj) {
    if (!weak_ref || !target_obj) return;

    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry *entry = malloc(sizeof(WeakRefEntry));

    if (!entry) {
        fprintf(stderr, "ERROR: Failed to allocate WeakRefEntry\n");
        return;
    }

    entry->weak_ref = weak_ref;
    entry->target_obj = target_obj;
    entry->next = weak_ref_table[hash];
    weak_ref_table[hash] = entry;

    // Increment weak count on target object
    ARC_ObjectHeader *target_header = __tsc_get_header(target_obj);
    if (target_header) {
        atomic_fetch_add(&target_header->weak_count, 1);
    }

    // Update statistics
    g_weak_ref_stats.total_registrations++;
    g_weak_ref_stats.current_active_refs++;
    if (g_weak_ref_stats.current_active_refs > g_weak_ref_stats.peak_active_refs) {
        g_weak_ref_stats.peak_active_refs = g_weak_ref_stats.current_active_refs;
    }

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Registered weak reference %p -> %p (active: %zu)\n", 
           weak_ref, target_obj, g_weak_ref_stats.current_active_refs);
#endif
}

// Unregister a weak reference
void __tsc_unregister_weak_ref(void *weak_ref) {
    if (!weak_ref) return;

    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry **entry_ptr = &weak_ref_table[hash];

    while (*entry_ptr) {
        if ((*entry_ptr)->weak_ref == weak_ref) {
            WeakRefEntry *to_free = *entry_ptr;
            void *target_obj = to_free->target_obj;
            *entry_ptr = (*entry_ptr)->next;
            
            // Decrement weak count on target object
            ARC_ObjectHeader *target_header = __tsc_get_header(target_obj);
            if (target_header) {
                atomic_fetch_sub(&target_header->weak_count, 1);
            }
            
            free(to_free);

            // Update statistics
            g_weak_ref_stats.total_unregistrations++;
            g_weak_ref_stats.current_active_refs--;

#ifdef TSC_ARC_DEBUG
            printf("DEBUG: Unregistered weak reference %p (active: %zu)\n", 
                   weak_ref, g_weak_ref_stats.current_active_refs);
#endif
            return;
        }
        entry_ptr = &(*entry_ptr)->next;
    }
}

// Check if weak reference is still valid
bool __tsc_is_weak_ref_valid(void *weak_ref) {
    if (!weak_ref) return false;

    g_weak_ref_stats.total_valid_checks++;

    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry *entry = weak_ref_table[hash];

    while (entry) {
        if (entry->weak_ref == weak_ref) {
            // Check if target object is still alive
            ARC_ObjectHeader *header = __tsc_get_header(entry->target_obj);
            return atomic_load(&header->ref_count) > 0;
        }
        entry = entry->next;
    }

    return false;
}

// Get target object from weak reference (if still alive)
void *__tsc_weak_ref_get_target(void *weak_ref) {
    if (!weak_ref) return NULL;

    g_weak_ref_stats.total_target_gets++;

    size_t hash = weak_ref_hash(weak_ref);
    WeakRefEntry *entry = weak_ref_table[hash];

    while (entry) {
        if (entry->weak_ref == weak_ref) {
            // Check if target object is still alive
            ARC_ObjectHeader *header = __tsc_get_header(entry->target_obj);
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
void __tsc_cleanup_weak_refs(void *target_obj) {
    if (!target_obj) return;

    size_t cleaned_count = 0;

    // Iterate through all weak reference entries
    for (size_t i = 0; i < WEAK_REF_TABLE_SIZE; i++) {
        WeakRefEntry **entry_ptr = &weak_ref_table[i];

        while (*entry_ptr) {
            if ((*entry_ptr)->target_obj == target_obj) {
                WeakRefEntry *to_free = *entry_ptr;
                *entry_ptr = (*entry_ptr)->next;

                cleaned_count++;

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

    // Update statistics
    g_weak_ref_stats.total_cleanups++;
    g_weak_ref_stats.current_active_refs -= cleaned_count;

#ifdef TSC_ARC_DEBUG
    if (cleaned_count > 0) {
        printf("DEBUG: Cleaned up %zu weak references for object %p (active: %zu)\n", 
               cleaned_count, target_obj, g_weak_ref_stats.current_active_refs);
    }
#endif
}

// Dump weak reference table (for debugging)
void __tsc_dump_weak_ref_table(void) {
    printf("=== Weak Reference Table ===\n");

    size_t total_entries = 0;
    for (size_t i = 0; i < WEAK_REF_TABLE_SIZE; i++) {
        WeakRefEntry *entry = weak_ref_table[i];
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

// Weak reference statistics functions
WeakRefStats __tsc_get_weak_ref_stats(void) {
    return g_weak_ref_stats;
}

void __tsc_reset_weak_ref_stats(void) {
    memset(&g_weak_ref_stats, 0, sizeof(g_weak_ref_stats));
}

void __tsc_print_weak_ref_stats(void) {
    printf("=== Weak Reference Statistics ===\n");
    printf("Total registrations: %zu\n", g_weak_ref_stats.total_registrations);
    printf("Total unregistrations: %zu\n", g_weak_ref_stats.total_unregistrations);
    printf("Total valid checks: %zu\n", g_weak_ref_stats.total_valid_checks);
    printf("Total target gets: %zu\n", g_weak_ref_stats.total_target_gets);
    printf("Total cleanups: %zu\n", g_weak_ref_stats.total_cleanups);
    printf("Current active refs: %zu\n", g_weak_ref_stats.current_active_refs);
    printf("Peak active refs: %zu\n", g_weak_ref_stats.peak_active_refs);
    printf("==================================\n");
}

// Clear all weak references (for testing)
void __tsc_clear_all_weak_refs(void) {
    for (size_t i = 0; i < WEAK_REF_TABLE_SIZE; i++) {
        WeakRefEntry *entry = weak_ref_table[i];
        while (entry) {
            WeakRefEntry *to_free = entry;
            entry = entry->next;
            free(to_free);
        }
        weak_ref_table[i] = NULL;
    }
    
    // Reset statistics
    memset(&g_weak_ref_stats, 0, sizeof(g_weak_ref_stats));
}
