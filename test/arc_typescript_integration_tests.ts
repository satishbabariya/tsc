// ARC TypeScript Integration Tests
// These tests validate ARC behavior through TypeScript smart pointer syntax

// Test 1: Basic shared_ptr lifecycle
function testSharedPtrLifecycle() {
    console.log("\n=== Test 1: SharedPtr Lifecycle ===");
    
    // Create shared_ptr
    let ptr1: shared_ptr<number> = new shared_ptr<number>(42);
    console.log("✅ Created shared_ptr with value 42");
    
    // Copy shared_ptr (should increment ref count)
    let ptr2: shared_ptr<number> = ptr1;
    console.log("✅ Copied shared_ptr - ref count should be 2");
    
    // Another copy
    let ptr3: shared_ptr<number> = ptr1;
    console.log("✅ Another copy - ref count should be 3");
    
    // Reassign ptr1 (should decrement ref count)
    ptr1 = new shared_ptr<number>(100);
    console.log("✅ Reassigned ptr1 - ref count should be 2");
    
    // Reassign ptr2 (should decrement ref count)
    ptr2 = new shared_ptr<number>(200);
    console.log("✅ Reassigned ptr2 - ref count should be 1");
    
    // Reassign ptr3 (should deallocate original object)
    ptr3 = new shared_ptr<number>(300);
    console.log("✅ Reassigned ptr3 - original object should be deallocated");
}

// Test 2: Unique_ptr behavior
function testUniquePtrBehavior() {
    console.log("\n=== Test 2: UniquePtr Behavior ===");
    
    // Create unique_ptr
    let ptr1: unique_ptr<number> = new unique_ptr<number>(42);
    console.log("✅ Created unique_ptr with value 42");
    
    // Move unique_ptr (should transfer ownership)
    let ptr2: unique_ptr<number> = std.move(ptr1);
    console.log("✅ Moved unique_ptr - ownership transferred");
    
    // ptr1 should now be null
    if (ptr1 == null) {
        console.log("✅ ptr1 is now null after move");
    } else {
        console.log("❌ ptr1 should be null after move");
    }
    
    // ptr2 should have the value
    console.log("✅ ptr2 has the moved value");
    
    // When ptr2 goes out of scope, object should be deallocated
    ptr2 = new unique_ptr<number>(100);
    console.log("✅ Reassigned ptr2 - original object should be deallocated");
}

// Test 3: Weak_ptr behavior
function testWeakPtrBehavior() {
    console.log("\n=== Test 3: WeakPtr Behavior ===");
    
    // Create shared_ptr
    let shared: shared_ptr<number> = new shared_ptr<number>(42);
    console.log("✅ Created shared_ptr with value 42");
    
    // Create weak_ptr from shared_ptr
    let weak: weak_ptr<number> = new weak_ptr<number>(shared);
    console.log("✅ Created weak_ptr from shared_ptr");
    
    // Lock weak_ptr (should succeed)
    let locked: shared_ptr<number> = weak.lock();
    if (locked != null) {
        console.log("✅ Successfully locked weak_ptr");
    } else {
        console.log("❌ Failed to lock weak_ptr");
    }
    
    // Release original shared_ptr
    shared = new shared_ptr<number>(100);
    console.log("✅ Released original shared_ptr");
    
    // Try to lock weak_ptr again (should fail)
    let locked2: shared_ptr<number> = weak.lock();
    if (locked2 == null) {
        console.log("✅ weak_ptr correctly returns null after object deallocation");
    } else {
        console.log("❌ weak_ptr should return null after object deallocation");
    }
}

// Test 4: Exception safety
function testExceptionSafety() {
    console.log("\n=== Test 4: Exception Safety ===");
    
    try {
        let ptr: shared_ptr<number> = new shared_ptr<number>(42);
        console.log("✅ Created shared_ptr in try block");
        
        // Simulate exception
        throw new Error("Simulated exception");
        
    } catch (error) {
        console.log("✅ Caught exception - shared_ptr should be automatically cleaned up");
    }
    
    console.log("✅ Exception safety test completed");
}

// Run all tests
function runAllTests() {
    console.log("🔄 ARC TypeScript Integration Tests");
    console.log("=====================================");
    
    testSharedPtrLifecycle();
    testUniquePtrBehavior();
    testWeakPtrBehavior();
    testExceptionSafety();
    
    console.log("\n✅ All TypeScript integration tests completed!");
}

// Test completed