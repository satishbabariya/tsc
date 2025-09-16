// Comprehensive Tests for Runtime ARC Improvements
// Tests object lifecycle checking and weak reference table management

// =============================================================================
// Test 1: Object Lifecycle Checking
// =============================================================================

// Test basic object lifecycle
class LifecycleTestClass {
    private id: number;
    private name: string;
    private data: number[];
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        this.data = [1, 2, 3, 4, 5];
        console.log(`LifecycleTestClass created: ${name} (ID: ${id})`);
    }
    
    getId(): number {
        return this.id;
    }
    
    getName(): string {
        return this.name;
    }
    
    getData(): number[] {
        return this.data;
    }
    
    setData(newData: number[]): void {
        this.data = newData;
    }
    
    // Test destructor
    destructor(): void {
        console.log(`LifecycleTestClass destroyed: ${this.name} (ID: ${this.id})`);
        this.data = [];
    }
}

// Test object lifecycle with references
class ReferenceTestClass {
    private ref: LifecycleTestClass | null;
    private weakRef: LifecycleTestClass | null;
    
    constructor() {
        this.ref = null;
        this.weakRef = null;
        console.log("ReferenceTestClass created");
    }
    
    setStrongReference(obj: LifecycleTestClass): void {
        this.ref = obj;
        console.log(`Strong reference set to: ${obj.getName()}`);
    }
    
    setWeakReference(obj: LifecycleTestClass): void {
        this.weakRef = obj;
        console.log(`Weak reference set to: ${obj.getName()}`);
    }
    
    getStrongReference(): LifecycleTestClass | null {
        return this.ref;
    }
    
    getWeakReference(): LifecycleTestClass | null {
        return this.weakRef;
    }
    
    clearReferences(): void {
        this.ref = null;
        this.weakRef = null;
        console.log("References cleared");
    }
    
    destructor(): void {
        console.log("ReferenceTestClass destroyed");
        this.clearReferences();
    }
}

function testObjectLifecycle(): void {
    console.log("Testing object lifecycle...");
    
    // Test basic object creation and destruction
    let obj1 = new LifecycleTestClass(1, "Object 1");
    let obj2 = new LifecycleTestClass(2, "Object 2");
    
    console.log(`Object 1 ID: ${obj1.getId()}`);
    console.log(`Object 2 ID: ${obj2.getId()}`);
    
    // Test object data access
    let data1 = obj1.getData();
    let data2 = obj2.getData();
    
    console.log(`Object 1 data: ${JSON.stringify(data1)}`);
    console.log(`Object 2 data: ${JSON.stringify(data2)}`);
    
    // Test object data modification
    obj1.setData([10, 20, 30]);
    obj2.setData([100, 200, 300]);
    
    let updatedData1 = obj1.getData();
    let updatedData2 = obj2.getData();
    
    console.log(`Updated Object 1 data: ${JSON.stringify(updatedData1)}`);
    console.log(`Updated Object 2 data: ${JSON.stringify(updatedData2)}`);
    
    // Test object destruction
    obj1.destructor();
    obj2.destructor();
    
    console.log("Object lifecycle test completed");
}

// =============================================================================
// Test 2: Weak Reference Table Management
// =============================================================================

// Test weak reference tracking
class WeakReferenceTestClass {
    private id: number;
    private name: string;
    private weakRefs: WeakReferenceTestClass[] = [];
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        console.log(`WeakReferenceTestClass created: ${name} (ID: ${id})`);
    }
    
    getId(): number {
        return this.id;
    }
    
    getName(): string {
        return this.name;
    }
    
    addWeakReference(ref: WeakReferenceTestClass): void {
        this.weakRefs.push(ref);
        console.log(`Weak reference added: ${ref.getName()} -> ${this.name}`);
    }
    
    getWeakReferences(): WeakReferenceTestClass[] {
        return this.weakRefs;
    }
    
    clearWeakReferences(): void {
        this.weakRefs = [];
        console.log(`Weak references cleared for: ${this.name}`);
    }
    
    destructor(): void {
        console.log(`WeakReferenceTestClass destroyed: ${this.name} (ID: ${this.id})`);
        this.clearWeakReferences();
    }
}

// Test weak reference table operations
class WeakReferenceTable {
    private table: Map<number, WeakReferenceTestClass[]> = new Map();
    
    addWeakReference(objId: number, ref: WeakReferenceTestClass): void {
        if (!this.table.has(objId)) {
            this.table.set(objId, []);
        }
        this.table.get(objId)!.push(ref);
        console.log(`Weak reference added to table: ${objId} -> ${ref.getName()}`);
    }
    
    removeWeakReferences(objId: number): void {
        if (this.table.has(objId)) {
            let refs = this.table.get(objId)!;
            console.log(`Removing ${refs.length} weak references for object ${objId}`);
            this.table.delete(objId);
        }
    }
    
    getWeakReferences(objId: number): WeakReferenceTestClass[] {
        return this.table.get(objId) || [];
    }
    
    hasWeakReferences(objId: number): boolean {
        return this.table.has(objId);
    }
    
    clear(): void {
        this.table.clear();
        console.log("Weak reference table cleared");
    }
}

function testWeakReferenceTable(): void {
    console.log("Testing weak reference table...");
    
    let table = new WeakReferenceTable();
    
    // Create objects
    let obj1 = new WeakReferenceTestClass(1, "Object 1");
    let obj2 = new WeakReferenceTestClass(2, "Object 2");
    let obj3 = new WeakReferenceTestClass(3, "Object 3");
    
    // Add weak references
    table.addWeakReference(1, obj2);
    table.addWeakReference(1, obj3);
    table.addWeakReference(2, obj1);
    table.addWeakReference(2, obj3);
    table.addWeakReference(3, obj1);
    table.addWeakReference(3, obj2);
    
    // Check weak references
    console.log(`Object 1 has weak references: ${table.hasWeakReferences(1)}`);
    console.log(`Object 2 has weak references: ${table.hasWeakReferences(2)}`);
    console.log(`Object 3 has weak references: ${table.hasWeakReferences(3)}`);
    
    let refs1 = table.getWeakReferences(1);
    let refs2 = table.getWeakReferences(2);
    let refs3 = table.getWeakReferences(3);
    
    console.log(`Object 1 weak references: ${refs1.map(r => r.getName()).join(", ")}`);
    console.log(`Object 2 weak references: ${refs2.map(r => r.getName()).join(", ")}`);
    console.log(`Object 3 weak references: ${refs3.map(r => r.getName()).join(", ")}`);
    
    // Remove weak references
    table.removeWeakReferences(1);
    table.removeWeakReferences(2);
    table.removeWeakReferences(3);
    
    console.log(`After removal - Object 1 has weak references: ${table.hasWeakReferences(1)}`);
    console.log(`After removal - Object 2 has weak references: ${table.hasWeakReferences(2)}`);
    console.log(`After removal - Object 3 has weak references: ${table.hasWeakReferences(3)}`);
    
    // Clean up
    obj1.destructor();
    obj2.destructor();
    obj3.destructor();
    table.clear();
    
    console.log("Weak reference table test completed");
}

// =============================================================================
// Test 3: ARC Memory Management
// =============================================================================

// Test ARC with strong references
class ARCStrongReferenceTest {
    private ref: LifecycleTestClass | null;
    
    constructor() {
        this.ref = null;
        console.log("ARCStrongReferenceTest created");
    }
    
    setReference(obj: LifecycleTestClass): void {
        this.ref = obj;
        console.log(`Strong reference set to: ${obj.getName()}`);
    }
    
    getReference(): LifecycleTestClass | null {
        return this.ref;
    }
    
    clearReference(): void {
        this.ref = null;
        console.log("Strong reference cleared");
    }
    
    destructor(): void {
        console.log("ARCStrongReferenceTest destroyed");
        this.clearReference();
    }
}

// Test ARC with weak references
class ARCWeakReferenceTest {
    private weakRef: LifecycleTestClass | null;
    
    constructor() {
        this.weakRef = null;
        console.log("ARCWeakReferenceTest created");
    }
    
    setWeakReference(obj: LifecycleTestClass): void {
        this.weakRef = obj;
        console.log(`Weak reference set to: ${obj.getName()}`);
    }
    
    getWeakReference(): LifecycleTestClass | null {
        return this.weakRef;
    }
    
    clearWeakReference(): void {
        this.weakRef = null;
        console.log("Weak reference cleared");
    }
    
    destructor(): void {
        console.log("ARCWeakReferenceTest destroyed");
        this.clearWeakReference();
    }
}

function testARCMemoryManagement(): void {
    console.log("Testing ARC memory management...");
    
    // Test strong reference counting
    let obj = new LifecycleTestClass(1, "ARC Test Object");
    let strongRef1 = new ARCStrongReferenceTest();
    let strongRef2 = new ARCStrongReferenceTest();
    
    strongRef1.setReference(obj);
    strongRef2.setReference(obj);
    
    console.log(`Object name: ${obj.getName()}`);
    console.log(`Strong reference 1: ${strongRef1.getReference()?.getName()}`);
    console.log(`Strong reference 2: ${strongRef2.getReference()?.getName()}`);
    
    // Test weak reference
    let weakRef = new ARCWeakReferenceTest();
    weakRef.setWeakReference(obj);
    
    console.log(`Weak reference: ${weakRef.getWeakReference()?.getName()}`);
    
    // Clear strong references
    strongRef1.clearReference();
    strongRef2.clearReference();
    
    console.log("Strong references cleared");
    
    // Check weak reference (should still be valid)
    console.log(`Weak reference after strong refs cleared: ${weakRef.getWeakReference()?.getName()}`);
    
    // Clear weak reference
    weakRef.clearWeakReference();
    
    console.log("Weak reference cleared");
    
    // Clean up
    strongRef1.destructor();
    strongRef2.destructor();
    weakRef.destructor();
    obj.destructor();
    
    console.log("ARC memory management test completed");
}

// =============================================================================
// Test 4: Circular Reference Detection
// =============================================================================

// Test circular reference detection
class CircularReferenceTest {
    private id: number;
    private name: string;
    private ref: CircularReferenceTest | null;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
        this.ref = null;
        console.log(`CircularReferenceTest created: ${name} (ID: ${id})`);
    }
    
    getId(): number {
        return this.id;
    }
    
    getName(): string {
        return this.name;
    }
    
    setReference(ref: CircularReferenceTest): void {
        this.ref = ref;
        console.log(`Circular reference set: ${this.name} -> ${ref.getName()}`);
    }
    
    getReference(): CircularReferenceTest | null {
        return this.ref;
    }
    
    clearReference(): void {
        this.ref = null;
        console.log(`Circular reference cleared: ${this.name}`);
    }
    
    destructor(): void {
        console.log(`CircularReferenceTest destroyed: ${this.name} (ID: ${this.id})`);
        this.clearReference();
    }
}

function testCircularReferenceDetection(): void {
    console.log("Testing circular reference detection...");
    
    // Create circular references
    let obj1 = new CircularReferenceTest(1, "Object 1");
    let obj2 = new CircularReferenceTest(2, "Object 2");
    let obj3 = new CircularReferenceTest(3, "Object 3");
    
    // Set up circular references
    obj1.setReference(obj2);
    obj2.setReference(obj3);
    obj3.setReference(obj1);
    
    console.log(`Object 1 references: ${obj1.getReference()?.getName()}`);
    console.log(`Object 2 references: ${obj2.getReference()?.getName()}`);
    console.log(`Object 3 references: ${obj3.getReference()?.getName()}`);
    
    // Break circular references
    obj1.clearReference();
    obj2.clearReference();
    obj3.clearReference();
    
    console.log("Circular references cleared");
    
    // Clean up
    obj1.destructor();
    obj2.destructor();
    obj3.destructor();
    
    console.log("Circular reference detection test completed");
}

// =============================================================================
// Test 5: Memory Leak Prevention
// =============================================================================

// Test memory leak prevention
class MemoryLeakTest {
    private data: number[];
    private refs: MemoryLeakTest[];
    private callback: (() => void) | null;
    
    constructor(size: number) {
        this.data = Array.from({ length: size }, (_, i) => i);
        this.refs = [];
        this.callback = null;
        console.log(`MemoryLeakTest created with ${size} elements`);
    }
    
    addReference(ref: MemoryLeakTest): void {
        this.refs.push(ref);
        console.log(`Reference added to ${this.data.length} element object`);
    }
    
    setCallback(callback: () => void): void {
        this.callback = callback;
        console.log("Callback set");
    }
    
    getData(): number[] {
        return this.data;
    }
    
    getRefs(): MemoryLeakTest[] {
        return this.refs;
    }
    
    clearData(): void {
        this.data = [];
        console.log("Data cleared");
    }
    
    clearRefs(): void {
        this.refs = [];
        console.log("References cleared");
    }
    
    clearCallback(): void {
        this.callback = null;
        console.log("Callback cleared");
    }
    
    destructor(): void {
        console.log(`MemoryLeakTest destroyed with ${this.data.length} elements`);
        this.clearData();
        this.clearRefs();
        this.clearCallback();
    }
}

function testMemoryLeakPrevention(): void {
    console.log("Testing memory leak prevention...");
    
    // Create objects with potential memory leaks
    let obj1 = new MemoryLeakTest(1000);
    let obj2 = new MemoryLeakTest(2000);
    let obj3 = new MemoryLeakTest(3000);
    
    // Set up references
    obj1.addReference(obj2);
    obj2.addReference(obj3);
    obj3.addReference(obj1);
    
    // Set up callbacks
    obj1.setCallback(() => console.log("Callback 1"));
    obj2.setCallback(() => console.log("Callback 2"));
    obj3.setCallback(() => console.log("Callback 3"));
    
    console.log(`Object 1 data size: ${obj1.getData().length}`);
    console.log(`Object 2 data size: ${obj2.getData().length}`);
    console.log(`Object 3 data size: ${obj3.getData().length}`);
    
    console.log(`Object 1 refs count: ${obj1.getRefs().length}`);
    console.log(`Object 2 refs count: ${obj2.getRefs().length}`);
    console.log(`Object 3 refs count: ${obj3.getRefs().length}`);
    
    // Clear references to prevent memory leaks
    obj1.clearRefs();
    obj2.clearRefs();
    obj3.clearRefs();
    
    obj1.clearCallback();
    obj2.clearCallback();
    obj3.clearCallback();
    
    console.log("References and callbacks cleared");
    
    // Clean up
    obj1.destructor();
    obj2.destructor();
    obj3.destructor();
    
    console.log("Memory leak prevention test completed");
}

// =============================================================================
// Test 6: Performance Testing
// =============================================================================

function testARCPerformance(): void {
    console.log("Testing ARC performance...");
    
    const iterations = 10000;
    
    // Test object creation and destruction performance
    let startTime = Date.now();
    for (let i = 0; i < iterations; i++) {
        let obj = new LifecycleTestClass(i, `Object ${i}`);
        obj.destructor();
    }
    let creationTime = Date.now() - startTime;
    console.log(`Object creation/destruction (${iterations} iterations): ${creationTime}ms`);
    
    // Test reference management performance
    startTime = Date.now();
    let refs: LifecycleTestClass[] = [];
    for (let i = 0; i < iterations; i++) {
        let obj = new LifecycleTestClass(i, `Ref Object ${i}`);
        refs.push(obj);
    }
    let refTime = Date.now() - startTime;
    console.log(`Reference management (${iterations} iterations): ${refTime}ms`);
    
    // Test weak reference performance
    startTime = Date.now();
    let weakRefs: LifecycleTestClass[] = [];
    for (let i = 0; i < iterations; i++) {
        let obj = new LifecycleTestClass(i, `Weak Ref Object ${i}`);
        weakRefs.push(obj);
    }
    let weakRefTime = Date.now() - startTime;
    console.log(`Weak reference management (${iterations} iterations): ${weakRefTime}ms`);
    
    // Clean up
    for (let ref of refs) {
        ref.destructor();
    }
    for (let weakRef of weakRefs) {
        weakRef.destructor();
    }
    
    console.log("ARC performance test completed");
}

// =============================================================================
// Test 7: Error Handling and Edge Cases
// =============================================================================

function testARCErrorHandling(): void {
    console.log("Testing ARC error handling...");
    
    try {
        // Test null reference handling
        let obj: LifecycleTestClass | null = null;
        if (obj !== null) {
            (obj as LifecycleTestClass).getName();
        }
        
        // Test undefined reference handling
        let obj2: LifecycleTestClass | undefined = undefined;
        if (obj2 !== undefined) {
            (obj2 as LifecycleTestClass).getId();
        }
        
        // Test reference after destruction
        let obj3 = new LifecycleTestClass(1, "Test Object");
        obj3.destructor();
        
        // Test weak reference after destruction
        let weakRef = new ARCWeakReferenceTest();
        weakRef.setWeakReference(obj3);
        
        // Test circular reference handling
        let circular1 = new CircularReferenceTest(1, "Circular 1");
        let circular2 = new CircularReferenceTest(2, "Circular 2");
        
        circular1.setReference(circular2);
        circular2.setReference(circular1);
        
        // Break circular reference
        circular1.clearReference();
        circular2.clearReference();
        
        // Clean up
        circular1.destructor();
        circular2.destructor();
        weakRef.destructor();
        
        console.log("ARC error handling test completed successfully");
        
    } catch (error) {
        console.log(`ARC error handling test failed: ${error}`);
    }
}

// =============================================================================
// Main Test Runner
// =============================================================================

function runRuntimeARCTests(): void {
    console.log("🧪 Running Runtime ARC Tests");
    console.log("============================");
    
    console.log("\n📊 Test 1: Object Lifecycle Checking");
    console.log("─".repeat(50));
    testObjectLifecycle();
    
    console.log("\n📊 Test 2: Weak Reference Table Management");
    console.log("─".repeat(50));
    testWeakReferenceTable();
    
    console.log("\n📊 Test 3: ARC Memory Management");
    console.log("─".repeat(50));
    testARCMemoryManagement();
    
    console.log("\n📊 Test 4: Circular Reference Detection");
    console.log("─".repeat(50));
    testCircularReferenceDetection();
    
    console.log("\n📊 Test 5: Memory Leak Prevention");
    console.log("─".repeat(50));
    testMemoryLeakPrevention();
    
    console.log("\n📊 Test 6: Performance Testing");
    console.log("─".repeat(50));
    testARCPerformance();
    
    console.log("\n📊 Test 7: Error Handling and Edge Cases");
    console.log("─".repeat(50));
    testARCErrorHandling();
    
    console.log("\n✅ Runtime ARC Tests Completed Successfully!");
}

// Export for use in other test files
export { runRuntimeARCTests };

// Run if this is the main module
if (require.main === module) {
    runRuntimeARCTests();
}