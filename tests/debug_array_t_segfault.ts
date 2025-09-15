// Debug test to reproduce Array<T> segmentation faults

// Test 1: Simple Array<T> operations that work
let simpleArray: Array<number> = [];
console.log("Simple Array<T> created successfully");

// Test 2: Array<T> with push operation (causes segfault)
let testArray: Array<number> = [];
testArray.push(42);
console.log("Array<T> push operation completed");

// Test 3: Array<T> with length access (causes segfault)
let lengthArray: Array<number> = [];
lengthArray.push(1);
lengthArray.push(2);
console.log("Array<T> length:", lengthArray.length);

// Test 4: Array<T> with element access (causes segfault)
let accessArray: Array<number> = [];
accessArray.push(10);
accessArray.push(20);
console.log("Array<T> first element:", accessArray[0]);