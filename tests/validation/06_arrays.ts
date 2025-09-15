// Test array literals and indexing
function testArrays() {
    // Array literals
    let numbers = [1, 2, 3, 4, 5];
    let mixed = [10, "hello", true];
    let empty = [];
    let nested = [[1, 2], [3, 4]];
    
    // Array indexing
    let first = numbers[0];      // 1
    let second = numbers[1];     // 2
    let mixedStr = mixed[1];     // "hello"
    let nestedNum = nested[0][1]; // 2
    
    // Array assignment
    numbers[2] = 99;
    mixed[0] = 42;
    
    return first;
}