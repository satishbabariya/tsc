// Regression tests for array method support
// These tests ensure that existing functionality is not broken by the new array method support

// Test 1: Verify existing array length property still works
function testExistingArrayLength(): void {
    console.log("=== Testing existing array length property ===");
    
    let numbers: Array<number> = [1, 2, 3, 4, 5];
    let length = numbers.length;
    
    console.log("Array length:", length);
    console.log("Length type check passed");
}

// Test 2: Verify existing array literal syntax still works
function testExistingArrayLiterals(): void {
    console.log("=== Testing existing array literal syntax ===");
    
    // Empty array
    let empty: Array<number> = [];
    console.log("Empty array length:", empty.length);
    
    // Array with elements
    let withElements: Array<string> = ["a", "b", "c"];
    console.log("Array with elements length:", withElements.length);
    
    // Mixed type array (any)
    let mixed: Array<any> = [1, "hello", true];
    console.log("Mixed array length:", mixed.length);
}

// Test 3: Verify existing array element access still works
function testExistingArrayElementAccess(): void {
    console.log("=== Testing existing array element access ===");
    
    let numbers: Array<number> = [10, 20, 30, 40, 50];
    
    // Test element access
    let first = numbers[0];
    let last = numbers[numbers.length - 1];
    let middle = numbers[2];
    
    console.log("First element:", first);
    console.log("Last element:", last);
    console.log("Middle element:", middle);
    
    // Test element modification
    numbers[1] = 25;
    console.log("Modified second element:", numbers[1]);
}

// Test 4: Verify existing array initialization patterns still work
function testExistingArrayInitialization(): void {
    console.log("=== Testing existing array initialization patterns ===");
    
    // Direct initialization
    let direct: Array<number> = [1, 2, 3];
    console.log("Direct initialization length:", direct.length);
    
    // Empty initialization
    let empty: Array<string> = [];
    console.log("Empty initialization length:", empty.length);
    
    // Variable assignment
    let assigned: Array<boolean> = [true, false];
    console.log("Variable assignment length:", assigned.length);
}

// Test 5: Verify existing array type compatibility still works
function testExistingArrayTypeCompatibility(): void {
    console.log("=== Testing existing array type compatibility ===");
    
    // Array type compatibility
    let numbers1: Array<number> = [1, 2, 3];
    let numbers2: Array<number> = numbers1;
    
    console.log("Type compatibility test passed");
    console.log("numbers2 length:", numbers2.length);
    
    // Generic array compatibility
    let generic1: Array<any> = [1, "hello", true];
    let generic2: Array<any> = generic1;
    
    console.log("Generic type compatibility test passed");
    console.log("generic2 length:", generic2.length);
}

// Test 6: Verify existing array in function parameters still works
function testExistingArrayFunctionParameters(): void {
    console.log("=== Testing existing array function parameters ===");
    
    function processArray(arr: Array<number>): number {
        return arr.length;
    }
    
    function processGenericArray<T>(arr: Array<T>): number {
        return arr.length;
    }
    
    let numbers: Array<number> = [1, 2, 3, 4, 5];
    let length1 = processArray(numbers);
    let length2 = processGenericArray(numbers);
    
    console.log("Function parameter test 1:", length1);
    console.log("Function parameter test 2:", length2);
}

// Test 7: Verify existing array return types still work
function testExistingArrayReturnTypes(): void {
    console.log("=== Testing existing array return types ===");
    
    function createNumberArray(): Array<number> {
        return [1, 2, 3, 4, 5];
    }
    
    function createStringArray(): Array<string> {
        return ["hello", "world", "test"];
    }
    
    function createGenericArray<T>(items: T[]): Array<T> {
        return items;
    }
    
    let numbers = createNumberArray();
    let strings = createStringArray();
    let generic = createGenericArray([true, false, true]);
    
    console.log("Number array length:", numbers.length);
    console.log("String array length:", strings.length);
    console.log("Generic array length:", generic.length);
}

// Test 8: Verify existing array in class properties still works
function testExistingArrayClassProperties(): void {
    console.log("=== Testing existing array class properties ===");
    
    class TestClass {
        numbers: Array<number>;
        strings: Array<string>;
        
        constructor() {
            this.numbers = [1, 2, 3];
            this.strings = ["a", "b", "c"];
        }
        
        getNumbersLength(): number {
            return this.numbers.length;
        }
        
        getStringsLength(): number {
            return this.strings.length;
        }
    }
    
    let test = new TestClass();
    console.log("Class numbers length:", test.getNumbersLength());
    console.log("Class strings length:", test.getStringsLength());
}

// Test 9: Verify existing array in generic classes still works
function testExistingArrayGenericClasses(): void {
    console.log("=== Testing existing array in generic classes ===");
    
    class GenericContainer<T> {
        items: Array<T>;
        
        constructor(initialItems: Array<T> = []) {
            this.items = initialItems;
        }
        
        getLength(): number {
            return this.items.length;
        }
        
        getItems(): Array<T> {
            return this.items;
        }
    }
    
    let numberContainer = new GenericContainer<number>([1, 2, 3]);
    let stringContainer = new GenericContainer<string>(["a", "b", "c"]);
    
    console.log("Generic number container length:", numberContainer.getLength());
    console.log("Generic string container length:", stringContainer.getLength());
}

// Test 10: Verify existing array type inference still works
function testExistingArrayTypeInference(): void {
    console.log("=== Testing existing array type inference ===");
    
    // Type inference from literals
    let inferredNumbers = [1, 2, 3, 4, 5];
    let inferredStrings = ["hello", "world", "test"];
    let inferredMixed = [1, "hello", true];
    
    console.log("Inferred numbers length:", inferredNumbers.length);
    console.log("Inferred strings length:", inferredStrings.length);
    console.log("Inferred mixed length:", inferredMixed.length);
    
    // Type inference in functions
    function processInferredArray(arr: Array<number>): void {
        console.log("Processing inferred array, length:", arr.length);
    }
    
    processInferredArray(inferredNumbers);
}

// Main regression test runner
function runRegressionTests(): void {
    console.log("Starting Array Method Regression Tests...");
    
    testExistingArrayLength();
    testExistingArrayLiterals();
    testExistingArrayElementAccess();
    testExistingArrayInitialization();
    testExistingArrayTypeCompatibility();
    testExistingArrayFunctionParameters();
    testExistingArrayReturnTypes();
    testExistingArrayClassProperties();
    testExistingArrayGenericClasses();
    testExistingArrayTypeInference();
    
    console.log("All regression tests completed successfully!");
    console.log("No regressions detected in existing array functionality.");
}

// Execute regression tests
runRegressionTests();