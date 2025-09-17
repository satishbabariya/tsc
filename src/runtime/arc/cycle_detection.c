#include "arc_runtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Cycle detection node for object graph traversal
typedef struct CycleNode {
    void *obj; // Object pointer
    bool visited; // Visited flag for DFS
    bool in_stack; // In current path flag
    int discovery_time; // Discovery time for DFS
    int finish_time; // Finish time for DFS
    struct CycleNode *next; // Linked list
} CycleNode;

// Object reference information
typedef struct ObjectRef {
    void *obj; // Referenced object
    bool is_strong; // True if strong reference, false if weak
    size_t field_offset; // Offset of the field in the object
    struct ObjectRef *next; // Linked list
} ObjectRef;

// Extended cycle detection context
typedef struct {
    CycleNode *nodes; // List of all nodes
    size_t node_count; // Number of nodes
    size_t capacity; // Capacity of nodes array
    ObjectRef *refs; // List of object references
    size_t ref_count; // Number of references
    size_t ref_capacity; // Capacity of references array
    int time_counter; // Global time counter for DFS
} CycleDetectionContext;

static CycleDetectionContext g_cycle_context = {0};

// Initialize cycle detection context
static void init_cycle_context(void) {
    if (g_cycle_context.nodes) return; // Already initialized

    g_cycle_context.capacity = 1024;
    g_cycle_context.nodes = malloc(sizeof(CycleNode) * g_cycle_context.capacity);
    g_cycle_context.node_count = 0;

    g_cycle_context.ref_capacity = 2048;
    g_cycle_context.refs = malloc(sizeof(ObjectRef) * g_cycle_context.ref_capacity);
    g_cycle_context.ref_count = 0;

    g_cycle_context.time_counter = 0;

    if (!g_cycle_context.nodes || !g_cycle_context.refs) {
        fprintf(stderr, "ERROR: Failed to allocate cycle detection context\n");
        if (g_cycle_context.nodes) {
            free(g_cycle_context.nodes);
            g_cycle_context.nodes = NULL;
        }
        if (g_cycle_context.refs) {
            free(g_cycle_context.refs);
            g_cycle_context.refs = NULL;
        }
        g_cycle_context.capacity = 0;
        g_cycle_context.ref_capacity = 0;
    }
}

// Add object to cycle detection context
static bool add_object_to_context(void *obj) {
    if (!obj) return false;

    init_cycle_context();

    if (g_cycle_context.node_count >= g_cycle_context.capacity) {
        // Resize context
        size_t new_capacity = g_cycle_context.capacity * 2;
        CycleNode *new_nodes = realloc(g_cycle_context.nodes,
                                       sizeof(CycleNode) * new_capacity);

        if (!new_nodes) {
            fprintf(stderr, "ERROR: Failed to resize cycle detection context\n");
            return false;
        }

        g_cycle_context.nodes = new_nodes;
        g_cycle_context.capacity = new_capacity;
    }

    // Check if object already exists
    for (size_t i = 0; i < g_cycle_context.node_count; i++) {
        if (g_cycle_context.nodes[i].obj == obj) {
            return true; // Already exists
        }
    }

    // Add new object
    CycleNode *node = &g_cycle_context.nodes[g_cycle_context.node_count];
    node->obj = obj;
    node->visited = false;
    node->in_stack = false;
    node->next = NULL;

    g_cycle_context.node_count++;
    return true;
}

// Reset cycle detection context
static void reset_cycle_context(void) {
    for (size_t i = 0; i < g_cycle_context.node_count; i++) {
        g_cycle_context.nodes[i].visited = false;
        g_cycle_context.nodes[i].in_stack = false;
        g_cycle_context.nodes[i].discovery_time = 0;
        g_cycle_context.nodes[i].finish_time = 0;
    }
    g_cycle_context.ref_count = 0;
    g_cycle_context.time_counter = 0;
}

// Find object in context
static CycleNode *find_object_in_context(void *obj) {
    for (size_t i = 0; i < g_cycle_context.node_count; i++) {
        if (g_cycle_context.nodes[i].obj == obj) {
            return &g_cycle_context.nodes[i];
        }
    }
    return NULL;
}

// Add reference to context
static bool add_reference_to_context(void *from_obj, void *to_obj, bool is_strong, size_t field_offset) {
    if (!from_obj || !to_obj) return false;

    if (g_cycle_context.ref_count >= g_cycle_context.ref_capacity) {
        // Resize references array
        size_t new_capacity = g_cycle_context.ref_capacity * 2;
        ObjectRef *new_refs = realloc(g_cycle_context.refs, sizeof(ObjectRef) * new_capacity);

        if (!new_refs) {
            fprintf(stderr, "ERROR: Failed to resize references array\n");
            return false;
        }

        g_cycle_context.refs = new_refs;
        g_cycle_context.ref_capacity = new_capacity;
    }

    // Add new reference
    ObjectRef *ref = &g_cycle_context.refs[g_cycle_context.ref_count];
    ref->obj = to_obj;
    ref->is_strong = is_strong;
    ref->field_offset = field_offset;
    ref->next = NULL;

    g_cycle_context.ref_count++;
    return true;
}

// Get references from an object (simplified - in real implementation would use type info)
static void get_object_references(void *obj, void **refs, size_t *ref_count) {
    if (!obj || !refs || !ref_count) return;

    *ref_count = 0;

    // This is a simplified implementation
    // In a real implementation, we would:
    // 1. Get the object's type information
    // 2. Iterate through all pointer fields
    // 3. Check if each field points to an ARC object
    // 4. Add valid references to the refs array

    // For now, we'll assume objects have a simple structure
    // where the first few words might be pointers
    void **obj_ptr = (void **) obj;

    // Check first few potential pointer fields
    for (int i = 0; i < 8 && *ref_count < 16; i++) {
        void *potential_ref = obj_ptr[i];
        if (potential_ref && __tsc_is_arc_object(potential_ref)) {
            refs[*ref_count] = potential_ref;
            (*ref_count)++;
        }
    }
}

// Convert strong reference to weak reference
static bool convert_to_weak_reference(void *from_obj, void *to_obj, size_t field_offset) {
    if (!from_obj || !to_obj) return false;

    // Get the field pointer
    void **field_ptr = (void **) ((char *) from_obj + field_offset);

    // Create a weak reference
    ARC_ObjectHeader *header = __tsc_get_header(to_obj);
    if (!header) return false;

    // For now, we'll use a simplified approach
    // In a real implementation, we would:
    // 1. Create a proper weak reference structure
    // 2. Update the field to point to the weak reference
    // 3. Register the weak reference in the weak reference table

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Converting strong reference at offset %zu to weak reference\n", field_offset);
#endif

    // This is a placeholder - actual implementation would be more complex
    return true;
}

// DFS for cycle detection
static bool dfs_cycle_detection(CycleNode *node, void **cycle_path, size_t *path_length) {
    if (!node) return false;

    node->visited = true;
    node->in_stack = true;
    node->discovery_time = ++g_cycle_context.time_counter;

    // Add to path
    if (path_length && cycle_path) {
        cycle_path[*path_length] = node->obj;
        (*path_length)++;
    }

    // Get references from this object
    void *refs[16];
    size_t ref_count = 0;
    get_object_references(node->obj, refs, &ref_count);

    // Traverse all references
    for (size_t i = 0; i < ref_count; i++) {
        void *ref_obj = refs[i];
        if (!ref_obj) continue;

        // Add referenced object to context if not already present
        add_object_to_context(ref_obj);

        // Find the referenced object's node
        CycleNode *ref_node = find_object_in_context(ref_obj);
        if (!ref_node) continue;

        // Add reference to context
        add_reference_to_context(node->obj, ref_obj, true, i * sizeof(void *));

        if (!ref_node->visited) {
            // Recursively visit unvisited nodes
            if (dfs_cycle_detection(ref_node, cycle_path, path_length)) {
                return true; // Cycle found
            }
        } else if (ref_node->in_stack) {
            // Back edge found - cycle detected
#ifdef TSC_ARC_DEBUG
            printf("DEBUG: Cycle detected: back edge from %p to %p\n", node->obj, ref_obj);
#endif
            return true;
        }
    }

    node->in_stack = false;
    node->finish_time = ++g_cycle_context.time_counter;
    return false;
}

// Detect cycles in object graph
bool __tsc_has_cycles(void *obj) {
    if (!obj) return false;

    // Add object to context
    if (!add_object_to_context(obj)) {
        return false;
    }

    reset_cycle_context();

    CycleNode *start_node = find_object_in_context(obj);
    if (!start_node) return false;

    void *cycle_path[1024];
    size_t path_length = 0;

    bool has_cycle = dfs_cycle_detection(start_node, cycle_path, &path_length);

    if (has_cycle && path_length > 0) {
#ifdef TSC_ARC_DEBUG
        printf("DEBUG: Cycle detected involving %zu objects:\n", path_length);
        for (size_t i = 0; i < path_length; i++) {
            printf("  %p\n", cycle_path[i]);
        }
#endif
    }

    return has_cycle;
}

// Find the weakest link in a cycle (object with lowest reference count)
static CycleNode *find_weakest_link_in_cycle(CycleNode *cycle_start, CycleNode *cycle_end) {
    CycleNode *weakest = cycle_start;
    size_t min_ref_count = __tsc_get_ref_count(cycle_start->obj);

    // Find the node with the minimum reference count in the cycle
    for (size_t i = 0; i < g_cycle_context.node_count; i++) {
        CycleNode *node = &g_cycle_context.nodes[i];
        if (node->discovery_time >= cycle_start->discovery_time &&
            node->discovery_time <= cycle_end->discovery_time) {
            size_t ref_count = __tsc_get_ref_count(node->obj);
            if (ref_count < min_ref_count) {
                min_ref_count = ref_count;
                weakest = node;
            }
        }
    }

    return weakest;
}

// Break cycles by converting strong references to weak references
void __tsc_break_cycles(void *obj) {
    if (!obj) return;

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Breaking cycles for object %p\n", obj);
#endif

    // Initialize context
    init_cycle_context();
    if (!g_cycle_context.nodes) return;

    // Add object to context
    if (!add_object_to_context(obj)) {
        return;
    }

    // Reset context for cycle detection
    reset_cycle_context();

    // Detect cycles using DFS
    CycleNode *start_node = find_object_in_context(obj);
    if (!start_node) return;

    void *cycle_path[1024];
    size_t path_length = 0;

    bool has_cycle = dfs_cycle_detection(start_node, cycle_path, &path_length);

    if (!has_cycle) {
#ifdef TSC_ARC_DEBUG
        printf("DEBUG: No cycles detected for object %p\n", obj);
#endif
        return;
    }

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Cycle detected involving %zu objects\n", path_length);
#endif

    // Find the weakest link in the cycle
    CycleNode *weakest_link = NULL;
    size_t min_ref_count = SIZE_MAX;

    for (size_t i = 0; i < g_cycle_context.node_count; i++) {
        CycleNode *node = &g_cycle_context.nodes[i];
        if (node->visited) {
            size_t ref_count = __tsc_get_ref_count(node->obj);
            if (ref_count < min_ref_count) {
                min_ref_count = ref_count;
                weakest_link = node;
            }
        }
    }

    if (!weakest_link) return;

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Weakest link found: %p with ref count %zu\n", 
           weakest_link->obj, min_ref_count);
#endif

    // Convert strong references to weak references
    // We'll convert references that point to the weakest link
    for (size_t i = 0; i < g_cycle_context.ref_count; i++) {
        ObjectRef *ref = &g_cycle_context.refs[i];
        if (ref->obj == weakest_link->obj && ref->is_strong) {
            // Find the source object
            CycleNode *source_node = NULL;
            for (size_t j = 0; j < g_cycle_context.node_count; j++) {
                CycleNode *node = &g_cycle_context.nodes[j];
                if (node->obj == ref->obj) {
                    source_node = node;
                    break;
                }
            }

            if (source_node) {
                // Convert this strong reference to a weak reference
                convert_to_weak_reference(source_node->obj, ref->obj, ref->field_offset);

#ifdef TSC_ARC_DEBUG
                printf("DEBUG: Converted strong reference from %p to %p to weak reference\n",
                       source_node->obj, ref->obj);
#endif
            }
        }
    }

#ifdef TSC_ARC_DEBUG
    printf("DEBUG: Cycle breaking completed for object %p\n", obj);
#endif
}

// Clean up cycle detection context
void __tsc_cleanup_cycle_detection(void) {
    if (g_cycle_context.nodes) {
        free(g_cycle_context.nodes);
        g_cycle_context.nodes = NULL;
        g_cycle_context.node_count = 0;
        g_cycle_context.capacity = 0;
    }

    if (g_cycle_context.refs) {
        free(g_cycle_context.refs);
        g_cycle_context.refs = NULL;
        g_cycle_context.ref_count = 0;
        g_cycle_context.ref_capacity = 0;
    }

    g_cycle_context.time_counter = 0;
}

// Dump cycle detection context (for debugging)
void __tsc_dump_cycle_context(void) {
    printf("=== Cycle Detection Context ===\n");
    printf("Node count: %zu\n", g_cycle_context.node_count);
    printf("Node capacity: %zu\n", g_cycle_context.capacity);
    printf("Reference count: %zu\n", g_cycle_context.ref_count);
    printf("Reference capacity: %zu\n", g_cycle_context.ref_capacity);
    printf("Time counter: %d\n", g_cycle_context.time_counter);

    printf("\n--- Nodes ---\n");
    for (size_t i = 0; i < g_cycle_context.node_count; i++) {
        CycleNode *node = &g_cycle_context.nodes[i];
        printf("Node %zu: obj=%p, visited=%s, in_stack=%s, discovery=%d, finish=%d\n",
               i, node->obj, node->visited ? "true" : "false",
               node->in_stack ? "true" : "false",
               node->discovery_time, node->finish_time);
    }

    printf("\n--- References ---\n");
    for (size_t i = 0; i < g_cycle_context.ref_count; i++) {
        ObjectRef *ref = &g_cycle_context.refs[i];
        printf("Ref %zu: obj=%p, strong=%s, offset=%zu\n",
               i, ref->obj, ref->is_strong ? "true" : "false", ref->field_offset);
    }

    printf("==============================\n");
}
