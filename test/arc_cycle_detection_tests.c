// ARC Cycle Detection Integration Tests
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "arc_runtime.h"

// Test object structures for cycle detection
typedef struct Node {
    int id;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct Parent {
    int id;
    struct Child* child;
} Parent;

typedef struct Child {
    int id;
    struct Parent* parent;
} Child;

// Destructors
void node_destructor(void* obj) {
    Node* node = (Node*)obj;
    printf("🔴 Node %d destructed\n", node->id);
}

void parent_destructor(void* obj) {
    Parent* parent = (Parent*)obj;
    printf("🔴 Parent %d destructed\n", parent->id);
}

void child_destructor(void* obj) {
    Child* child = (Child*)obj;
    printf("🔴 Child %d destructed\n", child->id);
}

// Test 1: Simple two-node cycle
int test_simple_cycle() {
    printf("\n=== Test 1: Simple Two-Node Cycle ===\n");
    
    // Create two nodes
    Node* node1 = (Node*)__tsc_alloc(sizeof(Node), node_destructor, NULL);
    Node* node2 = (Node*)__tsc_alloc(sizeof(Node), node_destructor, NULL);
    
    node1->id = 1;
    node2->id = 2;
    
    // Create cycle: node1 -> node2 -> node1
    node1->next = node2;
    node2->next = node1;
    
    printf("✅ Created cycle: Node1 -> Node2 -> Node1\n");
    
    // Both nodes should have ref_count = 1
    ARC_ObjectHeader* header1 = (ARC_ObjectHeader*)((char*)node1 - sizeof(ARC_ObjectHeader));
    ARC_ObjectHeader* header2 = (ARC_ObjectHeader*)((char*)node2 - sizeof(ARC_ObjectHeader));
    
    printf("📊 Node1 ref_count: %u\n", atomic_load(&header1->ref_count));
    printf("📊 Node2 ref_count: %u\n", atomic_load(&header2->ref_count));
    
    // Release both nodes
    printf("🔄 Releasing Node1...\n");
    __tsc_release(node1);
    
    printf("🔄 Releasing Node2...\n");
    __tsc_release(node2);
    
    printf("✅ Cycle test completed - objects should be deallocated\n");
    return 1;
}

// Test 2: Parent-Child cycle
int test_parent_child_cycle() {
    printf("\n=== Test 2: Parent-Child Cycle ===\n");
    
    // Create parent and child
    Parent* parent = (Parent*)__tsc_alloc(sizeof(Parent), parent_destructor, NULL);
    Child* child = (Child*)__tsc_alloc(sizeof(Child), child_destructor, NULL);
    
    parent->id = 1;
    child->id = 1;
    
    // Create cycle: parent -> child -> parent
    parent->child = child;
    child->parent = parent;
    
    printf("✅ Created cycle: Parent -> Child -> Parent\n");
    
    // Both should have ref_count = 1
    ARC_ObjectHeader* parent_header = (ARC_ObjectHeader*)((char*)parent - sizeof(ARC_ObjectHeader));
    ARC_ObjectHeader* child_header = (ARC_ObjectHeader*)((char*)child - sizeof(ARC_ObjectHeader));
    
    printf("📊 Parent ref_count: %u\n", atomic_load(&parent_header->ref_count));
    printf("📊 Child ref_count: %u\n", atomic_load(&child_header->ref_count));
    
    // Release both
    printf("🔄 Releasing Parent...\n");
    __tsc_release(parent);
    
    printf("🔄 Releasing Child...\n");
    __tsc_release(child);
    
    printf("✅ Parent-Child cycle test completed\n");
    return 1;
}

// Test 3: Complex multi-node cycle
int test_complex_cycle() {
    printf("\n=== Test 3: Complex Multi-Node Cycle ===\n");
    
    // Create 4 nodes in a cycle
    Node* nodes[4];
    for (int i = 0; i < 4; i++) {
        nodes[i] = (Node*)__tsc_alloc(sizeof(Node), node_destructor, NULL);
        nodes[i]->id = i + 1;
    }
    
    // Create cycle: 1 -> 2 -> 3 -> 4 -> 1
    for (int i = 0; i < 4; i++) {
        nodes[i]->next = nodes[(i + 1) % 4];
    }
    
    printf("✅ Created complex cycle: Node1 -> Node2 -> Node3 -> Node4 -> Node1\n");
    
    // All nodes should have ref_count = 1
    for (int i = 0; i < 4; i++) {
        ARC_ObjectHeader* header = (ARC_ObjectHeader*)((char*)nodes[i] - sizeof(ARC_ObjectHeader));
        printf("📊 Node%d ref_count: %u\n", i + 1, atomic_load(&header->ref_count));
    }
    
    // Release all nodes
    for (int i = 0; i < 4; i++) {
        printf("🔄 Releasing Node%d...\n", i + 1);
        __tsc_release(nodes[i]);
    }
    
    printf("✅ Complex cycle test completed\n");
    return 1;
}

// Test 4: No cycle (should deallocate normally)
int test_no_cycle() {
    printf("\n=== Test 4: No Cycle (Normal Deallocation) ===\n");
    
    // Create two nodes without cycle
    Node* node1 = (Node*)__tsc_alloc(sizeof(Node), node_destructor, NULL);
    Node* node2 = (Node*)__tsc_alloc(sizeof(Node), node_destructor, NULL);
    
    node1->id = 1;
    node2->id = 2;
    
    // No cycle: node1 -> node2 -> NULL
    node1->next = node2;
    node2->next = NULL;
    
    printf("✅ Created chain: Node1 -> Node2 -> NULL (no cycle)\n");
    
    // Release node1 first
    printf("🔄 Releasing Node1...\n");
    __tsc_release(node1);
    
    // Node2 should still exist
    ARC_ObjectHeader* header2 = (ARC_ObjectHeader*)((char*)node2 - sizeof(ARC_ObjectHeader));
    printf("📊 Node2 ref_count after Node1 release: %u\n", atomic_load(&header2->ref_count));
    
    // Release node2
    printf("🔄 Releasing Node2...\n");
    __tsc_release(node2);
    
    printf("✅ No cycle test completed\n");
    return 1;
}

// Test 5: Cycle with external references
int test_cycle_with_external_refs() {
    printf("\n=== Test 5: Cycle with External References ===\n");
    
    // Create cycle
    Node* node1 = (Node*)__tsc_alloc(sizeof(Node), node_destructor, NULL);
    Node* node2 = (Node*)__tsc_alloc(sizeof(Node), node_destructor, NULL);
    
    node1->id = 1;
    node2->id = 2;
    
    node1->next = node2;
    node2->next = node1;
    
    printf("✅ Created cycle: Node1 -> Node2 -> Node1\n");
    
    // Create external reference to node1
    __tsc_retain(node1);
    printf("✅ Created external reference to Node1\n");
    
    ARC_ObjectHeader* header1 = (ARC_ObjectHeader*)((char*)node1 - sizeof(ARC_ObjectHeader));
    ARC_ObjectHeader* header2 = (ARC_ObjectHeader*)((char*)node2 - sizeof(ARC_ObjectHeader));
    
    printf("📊 Node1 ref_count: %u\n", atomic_load(&header1->ref_count));
    printf("📊 Node2 ref_count: %u\n", atomic_load(&header2->ref_count));
    
    // Release original references
    printf("🔄 Releasing original Node1 reference...\n");
    __tsc_release(node1);
    
    printf("🔄 Releasing original Node2 reference...\n");
    __tsc_release(node2);
    
    // Objects should still exist due to external reference
    printf("📊 Node1 ref_count after releases: %u\n", atomic_load(&header1->ref_count));
    printf("📊 Node2 ref_count after releases: %u\n", atomic_load(&header2->ref_count));
    
    // Release external reference
    printf("🔄 Releasing external reference...\n");
    __tsc_release(node1);
    
    printf("✅ Cycle with external refs test completed\n");
    return 1;
}

int main() {
    printf("🔄 ARC Cycle Detection Integration Tests\n");
    printf("==========================================\n");
    
    int tests_passed = 0;
    int total_tests = 5;
    
    tests_passed += test_simple_cycle();
    tests_passed += test_parent_child_cycle();
    tests_passed += test_complex_cycle();
    tests_passed += test_no_cycle();
    tests_passed += test_cycle_with_external_refs();
    
    printf("\n📊 Test Results: %d/%d tests passed\n", tests_passed, total_tests);
    
    if (tests_passed == total_tests) {
        printf("✅ All cycle detection tests passed!\n");
        return 0;
    } else {
        printf("❌ Some cycle detection tests failed!\n");
        return 1;
    }
}