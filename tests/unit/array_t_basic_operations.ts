// Basic operations test for Array<T> vs T[]
let arrayT: Array<number> = [];
let arrayBracket: number[] = [];

// Test basic operations
arrayT.push(1);
arrayT.push(2);
arrayBracket.push(3);
arrayBracket.push(4);

_print("Array<T> length:", arrayT.length);
_print("T[] length:", arrayBracket.length);

// Test element access
_print("Array<T> first element:", arrayT[0]);
_print("T[] first element:", arrayBracket[0]);