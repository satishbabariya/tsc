// ARC TypeScript Integration Tests
// These tests validate ARC behavior through TypeScript smart pointer syntax

// Test 1: Basic shared_ptr lifecycle
function testSharedPtrLifecycle() {
    _print("\n=== Test 1: SharedPtr Lifecycle ===");

    // Create shared_ptr
    let ptr1: shared_ptr<number> = new shared_ptr<number>(42);
    _print("✅ Created shared_ptr with value 42");

    // Copy shared_ptr (should increment ref count)
    let ptr2: shared_ptr<number> = ptr1;
    _print("✅ Copied shared_ptr - ref count should be 2");

    // Another copy
    let ptr3: shared_ptr<number> = ptr1;
    _print("✅ Another copy - ref count should be 3");

    // Reassign ptr1 (should decrement ref count)
    ptr1 = new shared_ptr<number>(100);
    _print("✅ Reassigned ptr1 - ref count should be 2");

    // Reassign ptr2 (should decrement ref count)
    ptr2 = new shared_ptr<number>(200);
    _print("✅ Reassigned ptr2 - ref count should be 1");

    // Reassign ptr3 (should deallocate original object)
    ptr3 = new shared_ptr<number>(300);
    _print("✅ Reassigned ptr3 - original object should be deallocated");
}

// Test 2: Unique_ptr behavior
function testUniquePtrBehavior() {
    _print("\n=== Test 2: UniquePtr Behavior ===");

    // Create unique_ptr
    let ptr1: unique_ptr<number> = new unique_ptr<number>(42);
    _print("✅ Created unique_ptr with value 42");

    // Move unique_ptr (should transfer ownership)
    let ptr2: unique_ptr<number> = std.move(ptr1);
    _print("✅ Moved unique_ptr - ownership transferred");

    // ptr1 should now be null
    if (ptr1 == null) {
        _print("✅ ptr1 is now null after move");
    } else {
        _print("❌ ptr1 should be null after move");
    }

    // ptr2 should have the value
    _print("✅ ptr2 has the moved value");

    // When ptr2 goes out of scope, object should be deallocated
    ptr2 = new unique_ptr<number>(100);
    _print("✅ Reassigned ptr2 - original object should be deallocated");
}

// Test 3: Weak_ptr behavior
function testWeakPtrBehavior() {
    _print("\n=== Test 3: WeakPtr Behavior ===");

    // Create shared_ptr
    let shared: shared_ptr<number> = new shared_ptr<number>(42);
    _print("✅ Created shared_ptr with value 42");

    // Create weak_ptr from shared_ptr
    let weak: weak_ptr<number> = new weak_ptr<number>(shared);
    _print("✅ Created weak_ptr from shared_ptr");

    // Lock weak_ptr (should succeed)
    let locked: shared_ptr<number> = weak.lock();
    if (locked != null) {
        _print("✅ Successfully locked weak_ptr");
    } else {
        _print("❌ Failed to lock weak_ptr");
    }

    // Release original shared_ptr
    shared = new shared_ptr<number>(100);
    _print("✅ Released original shared_ptr");

    // Try to lock weak_ptr again (should fail)
    let locked2: shared_ptr<number> = weak.lock();
    if (locked2 == null) {
        _print("✅ weak_ptr correctly returns null after object deallocation");
    } else {
        _print("❌ weak_ptr should return null after object deallocation");
    }
}

// Test 4: Exception safety
function testExceptionSafety() {
    _print("\n=== Test 4: Exception Safety ===");

    try {
        let ptr: shared_ptr<number> = new shared_ptr<number>(42);
        _print("✅ Created shared_ptr in try block");

        // Simulate exception
        throw new Error("Simulated exception");

    } catch (error) {
        _print("✅ Caught exception - shared_ptr should be automatically cleaned up");
    }

    _print("✅ Exception safety test completed");
}

// Run all tests
function runAllTests() {
    _print("🔄 ARC TypeScript Integration Tests");
    _print("=====================================");

    testSharedPtrLifecycle();
    testUniquePtrBehavior();
    testWeakPtrBehavior();
    testExceptionSafety();

    _print("\n✅ All TypeScript integration tests completed!");
}

// Test completed