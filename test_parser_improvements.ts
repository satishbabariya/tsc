// Test file for parser improvements
// This file tests the enhanced error recovery, context-aware lexing, and lookahead improvements

// Test 1: Basic comparison operators (should work now)
let a = 5;
let b = 10;
if (a < b) {
    console.log("a is less than b");
}

// Test 2: Generic function calls (should work now)
function identity<T>(value: T): T {
    return value;
}

let result = identity<number>(42);
let result2 = identity<string>("hello");

// Test 3: Complex expressions with comparisons
let x = (a < b) && (b > a);
let y = (a <= b) || (b >= a);

// Test 4: Type annotations (should work with context-aware parsing)
let typedVar: number = 42;
let typedArray: number[] = [1, 2, 3];
let unionType: string | number = "hello";

// Test 5: Generic classes
class Container<T> {
    value: T;
    constructor(v: T) {
        this.value = v;
    }
    
    getValue(): T {
        return this.value;
    }
}

let numberContainer = new Container<number>(42);
let stringContainer = new Container<string>("hello");

// Test 6: Mixed cases that previously caused issues
let comparison = identity<number>(5) < 10;
let complex = (a < b) ? identity<string>("less") : identity<string>("greater");

// Test 7: Error recovery test (intentionally malformed)
let badSyntax = a < b &&; // This should recover gracefully

// Test 8: Nested comparisons
if ((a < b) && (b < 10) && (10 > a)) {
    console.log("All comparisons work");
}

// Test 9: Generic method calls
let container = new Container<number>(42);
let value = container.getValue();
let isPositive = value > 0;

// Test 10: Complex generic expressions
let result3 = identity<boolean>(a < b);
let result4 = identity<number>(a + b) < identity<number>(a * b);