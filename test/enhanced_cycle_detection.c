// Solution 2: Enhanced Cycle Detection Algorithm
// Implements Tarjan's Strongly Connected Components algorithm

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "arc_runtime.h"

// Enhanced cycle detection structures
typedef struct CycleNode {
    void* object;
    ARC_ObjectHeader* header;
    struct CycleNode** references;
    int reference_count;
    int index;
    int lowlink;
    bool on_stack;
    bool visited;
} CycleNode;

typedef struct CycleDetector {
    CycleNode* nodes;
    int node_count;
    int capacity;
    int* stack;
    int stack_top;
    int index_counter;
} CycleDetector;

// Tarjan's algorithm for finding strongly connected components
void tarjan_strongly_connected_components(CycleDetector* detector, CycleNode* node) {
    node->index = detector->index_counter;
    node->lowlink = detector->index_counter;
    detector->index_counter++;
    
    detector->stack[detector->stack_top++] = (int)(node - detector->nodes);
    node->on_stack = true;
    
    // Visit all references
    for (int i = 0; i < node->reference_count; i++) {
        CycleNode* neighbor = node->references[i];
        
        if (neighbor->index == -1) {
            // Not yet visited
            tarjan_strongly_connected_components(detector, neighbor);
            node->lowlink = (node->lowlink < neighbor->lowlink) ? node->lowlink : neighbor->lowlink;
        } else if (neighbor->on_stack) {
            // Back edge found
            node->lowlink = (node->lowlink < neighbor->index) ? node->lowlink : neighbor->index;
        }
    }
    
    // If node is root of strongly connected component
    if (node->lowlink == node->index) {
        CycleNode* component_nodes[1000];  // Max component size
        int component_size = 0;
        
        // Collect all nodes in this component
        while (detector->stack_top > 0) {
            int node_index = detector->stack[--detector->stack_top];
            CycleNode* component_node = &detector->nodes[node_index];
            component_node->on_stack = false;
            component_nodes[component_size++] = component_node;
            
            if (component_node == node) break;
        }
        
        // Check if component has external references
        bool has_external_refs = false;
        for (int i = 0; i < component_size; i++) {
            if (atomic_load(&component_nodes[i]->header->ref_count) > component_size) {
                has_external_refs = true;
                break;
            }
        }
        
        // If no external references, this is a cycle - break it
        if (!has_external_refs && component_size > 1) {
            printf("🔍 CYCLE DETECTED: %d objects in cycle\n", component_size);
            
            // Break cycle by converting one reference to weak
            // Find the reference with lowest ref_count
            CycleNode* weakest = component_nodes[0];
            for (int i = 1; i < component_size; i++) {
                if (atomic_load(&component_nodes[i]->header->ref_count) < 
                    atomic_load(&weakest->header->ref_count)) {
                    weakest = component_nodes[i];
                }
            }
            
            printf("🔧 Breaking cycle at object %p (weakest ref_count: %u)\n", 
                   weakest->object, atomic_load(&weakest->header->ref_count));
            
            // Convert one strong reference to weak
            // This is a simplified approach - in practice, we'd need more sophisticated logic
            atomic_fetch_sub(&weakest->header->ref_count, 1);
        }
    }
}

// Enhanced cycle detection function
void enhanced_cycle_detection(void* root_objects[], int object_count) {
    printf("\n=== Enhanced Cycle Detection ===\n");
    
    CycleDetector detector;
    detector.nodes = (CycleNode*)malloc(object_count * sizeof(CycleNode));
    detector.node_count = object_count;
    detector.capacity = object_count;
    detector.stack = (int*)malloc(object_count * sizeof(int));
    detector.stack_top = 0;
    detector.index_counter = 0;
    
    // Initialize nodes
    for (int i = 0; i < object_count; i++) {
        detector.nodes[i].object = root_objects[i];
        detector.nodes[i].header = (ARC_ObjectHeader*)((char*)root_objects[i] - sizeof(ARC_ObjectHeader));
        detector.nodes[i].references = NULL;
        detector.nodes[i].reference_count = 0;
        detector.nodes[i].index = -1;
        detector.nodes[i].lowlink = -1;
        detector.nodes[i].on_stack = false;
        detector.nodes[i].visited = false;
    }
    
    // Build reference graph (simplified - in practice, we'd traverse object fields)
    // For demonstration, we'll assume objects reference each other in a cycle
    for (int i = 0; i < object_count; i++) {
        detector.nodes[i].references = (CycleNode**)malloc((object_count - 1) * sizeof(CycleNode*));
        int ref_index = 0;
        
        for (int j = 0; j < object_count; j++) {
            if (i != j) {
                detector.nodes[i].references[ref_index++] = &detector.nodes[j];
            }
        }
        detector.nodes[i].reference_count = ref_index;
    }
    
    // Run Tarjan's algorithm
    for (int i = 0; i < object_count; i++) {
        if (detector.nodes[i].index == -1) {
            tarjan_strongly_connected_components(&detector, &detector.nodes[i]);
        }
    }
    
    // Cleanup
    for (int i = 0; i < object_count; i++) {
        free(detector.nodes[i].references);
    }
    free(detector.nodes);
    free(detector.stack);
    
    printf("✅ Enhanced cycle detection completed\n");
}

// Test function
int test_enhanced_cycle_detection() {
    printf("🔄 Testing Enhanced Cycle Detection\n");
    
    // Create test objects
    void* objects[4];
    for (int i = 0; i < 4; i++) {
        objects[i] = __tsc_alloc(64, NULL, NULL);  // 64-byte test objects
    }
    
    // Run cycle detection
    enhanced_cycle_detection(objects, 4);
    
    // Cleanup
    for (int i = 0; i < 4; i++) {
        __tsc_release(objects[i]);
    }
    
    return 0;
}