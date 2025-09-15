// Unit tests for array method support in PropertyAccess::visit()
// These tests verify that the semantic analyzer correctly identifies array methods
// and creates appropriate FunctionType objects

// Test 1: Basic array method identification
let testArray1: Array<number> = [];
let length1 = testArray1.length;  // Should return number type
let pushResult1 = testArray1.push(1, 2, 3);  // Should return number type (new length)
let popResult1 = testArray1.pop();  // Should return number type or undefined

// Test 2: Array method chaining and type inference
let testArray2: Array<string> = ["hello", "world"];
let shiftResult2 = testArray2.shift();  // Should return string type or undefined
let unshiftResult2 = testArray2.unshift("new", "items");  // Should return number type

// Test 3: Complex array methods
let testArray3: Array<boolean> = [true, false, true];
let concatResult3 = testArray3.concat([false]);  // Should return Array<boolean>
let sliceResult3 = testArray3.slice(0, 2);  // Should return Array<boolean>
let spliceResult3 = testArray3.splice(1, 1);  // Should return Array<boolean>
let indexResult3 = testArray3.indexOf(true);  // Should return number

// Test 4: Higher-order array methods
let testArray4: Array<number> = [1, 2, 3, 4, 5];
let forEachResult4 = testArray4.forEach((item) => _print(item));  // Should return void
let mapResult4 = testArray4.map((item) => item * 2);  // Should return Array<number>
let filterResult4 = testArray4.filter((item) => item > 2);  // Should return Array<number>
let reduceResult4 = testArray4.reduce((acc, item) => acc + item, 0);  // Should return number

// Test 5: Generic array types
let genericArray1: Array<any> = [1, "hello", true];
let genericLength1 = genericArray1.length;  // Should return number
let genericPush1 = genericArray1.push("test");  // Should return number

// Test 6: Array method with different element types
let stringArray: Array<string> = ["a", "b", "c"];
let numberArray: Array<number> = [1, 2, 3];
let booleanArray: Array<boolean> = [true, false];

// Test length property on different array types
let stringLength = stringArray.length;
let numberLength = numberArray.length;
let booleanLength = booleanArray.length;

// Test push method with correct element types
let stringPush = stringArray.push("d", "e");
let numberPush = numberArray.push(4, 5);
let booleanPush = booleanArray.push(true);

// Test pop method returns correct element types
let stringPop = stringArray.pop();
let numberPop = numberArray.pop();
let booleanPop = booleanArray.pop();

_print("Array method unit tests completed successfully");