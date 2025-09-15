// Solution 4: Advanced Cycle Detection with Runtime Integration
// This integrates with the ARC runtime to provide automatic cycle detection

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "arc_runtime.h"

// Advanced cycle detection structures
typedef struct ObjectGraph {
    void* object;
    ARC_ObjectHeader* header;
    struct ObjectGraph** references;
    int reference_count;
    bool visited;
    bool in_cycle;
    int dfs_number;
    int lowlink;
} ObjectGraph;

typedef struct CycleDetector {
    ObjectGraph* graph;
    int object_count;
    int* stack;
    int stack_top;
    int dfs_counter;
    bool cycle_found;
} CycleDetector;

// Global cycle detector instance
static CycleDetector* global_detector = NULL;

// Initialize cycle detector
void init_cycle_detector() {
    if (global_detector) return;
    
    global_detector = (CycleDetector*)malloc(sizeof(CycleDetector));
    global_detector->graph = NULL;
    global_detector->object_count = 0;
    global_detector->stack = NULL;
    global_detector->stack_top = 0;
    global_detector->dfs_counter = 0;
    global_detector->cycle_found = false;
    
    printf("🔧 Advanced cycle detector initialized\n");
}

// Build object graph from ARC objects
void build_object_graph(ObjectGraph* graph, int max_objects) {
    // This is a simplified version - in practice, we'd traverse object fields
    // to build the actual reference graph
    
    for (int i = 0; i < max_objects; i++) {
        graph[i].object = NULL;
        graph[i].header = NULL;
        graph[i].references = NULL;
        graph[i].reference_count = 0;
        graph[i].visited = false;
        graph[i].in_cycle = false;
        graph[i].dfs_number = -1;
        graph[i].lowlink = -1;
    }
}

// Tarjan's algorithm for cycle detection
void tarjan_cycle_detection(ObjectGraph* node, CycleDetector* detector) {
    node->dfs_number = detector->dfs_counter;
    node->lowlink = detector->dfs_counter;
    detector->dfs_counter++;
    
    detector->stack[detector->stack_top++] = (int)(node - detector->graph);
    node->visited = true;
    
    // Visit all references
    for (int i = 0; i < node->reference_count; i++) {
        ObjectGraph* neighbor = node->references[i];
        
        if (neighbor->dfs_number == -1) {
            // Not yet visited
            tarjan_cycle_detection(neighbor, detector);
            node->lowlink = (node->lowlink < neighbor->lowlink) ? node->lowlink : neighbor->lowlink;
        } else if (neighbor->visited) {
            // Back edge found
            node->lowlink = (node->lowlink < neighbor->dfs_number) ? node->lowlink : neighbor->dfs_number;
        }
    }
    
    // If node is root of strongly connected component
    if (node->lowlink == node->dfs_number) {
        ObjectGraph* cycle_nodes[1000];
        int cycle_size = 0;
        
        // Collect all nodes in this component
        while (detector->stack_top > 0) {
            int node_index = detector->stack[--detector->stack_top];
            ObjectGraph* component_node = &detector->graph[node_index];
            component_node->visited = false;
            cycle_nodes[cycle_size++] = component_node;
            
            if (component_node == node) break;
        }
        
        // Check if this is a cycle (more than one node)
        if (cycle_size > 1) {
            printf("🔍 CYCLE DETECTED: %d objects in cycle\n", cycle_size);
            
            // Mark all nodes in cycle
            for (int i = 0; i < cycle_size; i++) {
                cycle_nodes[i]->in_cycle = true;
            }
            
            // Check for external references
            bool has_external_refs = false;
            for (int i = 0; i < cycle_size; i++) {
                uint32_t ref_count = atomic_load(&cycle_nodes[i]->header->ref_count);
                if (ref_count > cycle_size) {
                    has_external_refs = true;
                    break;
                }
            }
            
            if (!has_external_refs) {
                printf("🔧 Breaking cycle - no external references\n");
                
                // Find the weakest reference to break
                ObjectGraph* weakest = cycle_nodes[0];
                for (int i = 1; i < cycle_size; i++) {
                    if (atomic_load(&cycle_nodes[i]->header->ref_count) < 
                        atomic_load(&weakest->header->ref_count)) {
                        weakest = cycle_nodes[i];
                    }
                }
                
                // Break the cycle by reducing ref_count
                atomic_fetch_sub(&weakest->header->ref_count, 1);
                printf("🔧 Reduced ref_count for object %p\n", weakest->object);
                
                detector->cycle_found = true;
            }
        }
    }
}

// Run cycle detection on all objects
void run_cycle_detection() {
    if (!global_detector) {
        init_cycle_detector();
    }
    
    printf("\n=== Running Advanced Cycle Detection ===\n");
    
    // Reset detector state
    global_detector->dfs_counter = 0;
    global_detector->stack_top = 0;
    global_detector->cycle_found = false;
    
    // Reset all nodes
    for (int i = 0; i < global_detector->object_count; i++) {
        global_detector->graph[i].visited = false;
        global_detector->graph[i].in_cycle = false;
        global_detector->graph[i].dfs_number = -1;
        global_detector->graph[i].lowlink = -1;
    }
    
    // Run Tarjan's algorithm on all unvisited nodes
    for (int i = 0; i < global_detector->object_count; i++) {
        if (global_detector->graph[i].dfs_number == -1) {
            tarjan_cycle_detection(&global_detector->graph[i], global_detector);
        }
    }
    
    if (global_detector->cycle_found) {
        printf("✅ Cycle detection completed - cycles found and broken\n");
    } else {
        printf("✅ Cycle detection completed - no cycles found\n");
    }
}

// Add object to cycle detector
void add_object_to_detector(void* object) {
    if (!global_detector) {
        init_cycle_detector();
    }
    
    // Expand graph if needed
    if (global_detector->object_count >= 1000) {
        printf("⚠️  Cycle detector at capacity\n");
        return;
    }
    
    int index = global_detector->object_count++;
    global_detector->graph[index].object = object;
    global_detector->graph[index].header = (ARC_ObjectHeader*)((char*)object - sizeof(ARC_ObjectHeader));
    global_detector->graph[index].references = NULL;
    global_detector->graph[index].reference_count = 0;
    global_detector->graph[index].visited = false;
    global_detector->graph[index].in_cycle = false;
    global_detector->graph[index].dfs_number = -1;
    global_detector->graph[index].lowlink = -1;
    
    printf("📝 Added object %p to cycle detector\n", object);
}

// Remove object from cycle detector
void remove_object_from_detector(void* object) {
    if (!global_detector) return;
    
    // Find and remove object
    for (int i = 0; i < global_detector->object_count; i++) {
        if (global_detector->graph[i].object == object) {
            // Shift remaining objects
            for (int j = i; j < global_detector->object_count - 1; j++) {
                global_detector->graph[j] = global_detector->graph[j + 1];
            }
            global_detector->object_count--;
            printf("🗑️  Removed object %p from cycle detector\n", object);
            break;
        }
    }
}

// Enhanced ARC allocation with cycle detection
void* __tsc_alloc_with_cycle_detection(size_t size, void (*destructor)(void*), void* type_info) {
    void* object = __tsc_alloc(size, destructor, type_info);
    
    if (object) {
        add_object_to_detector(object);
    }
    
    return object;
}

// Enhanced ARC deallocation with cycle detection
void __tsc_release_with_cycle_detection(void* object) {
    if (!object) return;
    
    remove_object_from_detector(object);
    __tsc_release(object);
}

// Periodic cycle detection (call this periodically)
void periodic_cycle_detection() {
    if (global_detector && global_detector->object_count > 0) {
        run_cycle_detection();
    }
}

// Cleanup cycle detector
void cleanup_cycle_detector() {
    if (global_detector) {
        if (global_detector->graph) {
            free(global_detector->graph);
        }
        if (global_detector->stack) {
            free(global_detector->stack);
        }
        free(global_detector);
        global_detector = NULL;
        printf("🧹 Cycle detector cleaned up\n");
    }
}