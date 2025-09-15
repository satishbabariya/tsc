
// Array destructuring testing
function testArrayDestructuring() {
    // Basic array destructuring
    const numbers = [1, 2, 3, 4, 5];
    const [first, second, third] = numbers;
    
    console.log("First:", first);
    console.log("Second:", second);
    console.log("Third:", third);
    
    // Array destructuring with rest
    const [a, b, ...rest] = numbers;
    console.log("A:", a);
    console.log("B:", b);
    console.log("Rest:", rest);
    
    // Array destructuring with default values
    const [x = 0, y = 0, z = 0] = [1, 2];
    console.log("X:", x);
    console.log("Y:", y);
    console.log("Z:", z);
    
    // Array destructuring with skipped elements
    const [, , thirdElement] = numbers;
    console.log("Third element:", thirdElement);
    
    // Array destructuring in function parameters
    function processArray([first, second, ...rest]: number[]) {
        console.log("First:", first);
        console.log("Second:", second);
        console.log("Rest:", rest);
    }
    
    processArray([10, 20, 30, 40, 50]);
}

// Test with different data types
function testArrayDestructuringTypes() {
    const mixedArray = ["hello", 42, true, { name: "test" }];
    const [str, num, bool, obj] = mixedArray;
    
    console.log("String:", str);
    console.log("Number:", num);
    console.log("Boolean:", bool);
    console.log("Object:", obj);
}

// Test with nested arrays
function testNestedArrayDestructuring() {
    const nestedArray = [[1, 2], [3, 4], [5, 6]];
    const [[a, b], [c, d], [e, f]] = nestedArray;
    
    console.log("A:", a, "B:", b);
    console.log("C:", c, "D:", d);
    console.log("E:", e, "F:", f);
}

// Run tests
testArrayDestructuring();
testArrayDestructuringTypes();
testNestedArrayDestructuring();
