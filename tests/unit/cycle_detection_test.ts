// Cycle Detection Test
// This test creates reference cycles to verify cycle detection works

class CyclicNode {
    id: number;
    next: CyclicNode | null;

    constructor(id: number) {
        this.id = id;
        this.next = null;
        _print(`CyclicNode ${id} created`);
    }

~

    CyclicNode() {
        _print(`CyclicNode ${this.id} destroyed`);
    }

    setNext(node: CyclicNode) {
        this.next = node;
    }
}

function testCycleDetection() {
    _print("=== Cycle Detection Test ===");

    let node1 = new CyclicNode(1);
    let node2 = new CyclicNode(2);
    let node3 = new CyclicNode(3);

    // Create a cycle: 1 -> 2 -> 3 -> 1
    node1.setNext(node2);
    node2.setNext(node3);
    node3.setNext(node1);

    _print("Cycle created: 1 -> 2 -> 3 -> 1");
    _print("Going out of scope - cycle should be detected and broken");
}

testCycleDetection();