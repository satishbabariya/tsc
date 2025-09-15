// Strong Reference Test - Demonstrates ARC behavior
function testStrongReferences() {
    console.log("=== Strong Reference Test ===");
    
    // Create initial shared_ptr
    let original: shared_ptr<number> = new shared_ptr<number>(42);
    console.log("Created original shared_ptr with value 42");
    
    // Create strong references
    let copy1: shared_ptr<number> = original;  // ref_count = 2
    let copy2: shared_ptr<number> = original;  // ref_count = 3
    let copy3: shared_ptr<number> = original;  // ref_count = 4
    
    console.log("Created 3 strong references - ref_count = 4");
    
    // Reassign some references
    copy1 = new shared_ptr<number>(100);  // ref_count = 3
    copy2 = new shared_ptr<number>(200);  // ref_count = 2
    
    console.log("Reassigned copy1 and copy2 - ref_count = 2");
    
    // Reassign original
    original = new shared_ptr<number>(300);  // ref_count = 1
    
    console.log("Reassigned original - ref_count = 1");
    
    // Reassign last copy - OBJECT DEALLOCATED!
    copy3 = new shared_ptr<number>(400);  // ref_count = 0 → DEALLOCATION
    
    console.log("Reassigned copy3 - ref_count = 0, object deallocated");
}

// Test with function scope
function testFunctionScope() {
    console.log("\n=== Function Scope Test ===");
    
    let local_ptr: shared_ptr<number> = new shared_ptr<number>(999);
    console.log("Created local_ptr in function");
    
    // When function ends, local_ptr goes out of scope
    // This triggers __tsc_release() and deallocates the object
    console.log("Function ending - local_ptr will be deallocated");
}

testStrongReferences();
testFunctionScope();