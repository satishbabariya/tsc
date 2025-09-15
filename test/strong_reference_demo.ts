// Strong Reference Demo - Shows ARC lifecycle
let ptr1: shared_ptr<number> = new shared_ptr<number>(42);
console.log("Created shared_ptr with value 42");

// This creates a STRONG reference - increments ref count
let ptr2: shared_ptr<number> = ptr1;  // Copy constructor - ref count becomes 2
console.log("Copied shared_ptr - ref count is now 2");

// Another strong reference
let ptr3: shared_ptr<number> = ptr1;  // Copy constructor - ref count becomes 3
console.log("Another copy - ref count is now 3");

// When we reassign ptr1, it releases its reference
ptr1 = new shared_ptr<number>(100);  // ref count becomes 2 (ptr1 releases, ptr2/ptr3 still hold)
console.log("Reassigned ptr1 - ref count is now 2");

// When ptr2 goes out of scope or is reassigned
ptr2 = new shared_ptr<number>(200);  // ref count becomes 1 (only ptr3 holds it)
console.log("Reassigned ptr2 - ref count is now 1");

// When ptr3 goes out of scope or is reassigned
ptr3 = new shared_ptr<number>(300);  // ref count becomes 0 - OBJECT IS DEALLOCATED!
console.log("Reassigned ptr3 - ref count is 0, original object deallocated");