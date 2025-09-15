// Basic operations test for Array<T> vs T[]
let arrayT: Array<number> = [];
let arrayBracket: number[] = [];

// Test basic operations
arrayT.push(1);
arrayT.push(2);
arrayBracket.push(3);
arrayBracket.push(4);

console.log("Array<T> length:", arrayT.length);
console.log("T[] length:", arrayBracket.length);

// Test element access
console.log("Array<T> first element:", arrayT[0]);
console.log("T[] first element:", arrayBracket[0]);