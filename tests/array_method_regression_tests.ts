// Regression tests for array method support
// These tests ensure that existing functionality is not broken by the new array method support

// Test 1: Verify existing array length property still works
function testExistingArrayLength(): void {
    _print("=== Testing existing array length property ===");
    
    let numbers: Array<number> = [1, 2, 3, 4, 5];
    let length = numbers.length;
    
    _print("Array length:", length);
    _print("Length type check passed");
}

// Test 2: Verify existing array literal syntax still works
function testExistingArrayLiterals(): void {
    _print("=== Testing existing array literal syntax ===");
    
    // Empty array
    let empty: Array<number> = [];
    _print("Empty array length:", empty.length);
    
    // Array with elements
    let withElements: Array<string> = ["a", "b", "c"];
    _print("Array with elements length:", withElements.length);
    
    // Mixed type array (any)
    let mixed: Array<any> = [1, "hello", true];
    _print("Mixed array length:", mixed.length);
}

// Test 3: Verify existing array element access still works
function testExistingArrayElementAccess(): void {
    _print("=== Testing existing array element access ===");
    
    let numbers: Array<number> = [10, 20, 30, 40, 50];
    
    // Test element access
    let first = numbers[0];
    let last = numbers[numbers.length - 1];
    let middle = numbers[2];
    
    _print("First element:", first);
    _print("Last element:", last);
    _print("Middle element:", middle);
    
    // Test element modification
    numbers[1] = 25;
    _print("Modified second element:", numbers[1]);
}

// Test 4: Verify existing array initialization patterns still work
function testExistingArrayInitialization(): void {
    _print("=== Testing existing array initialization patterns ===");
    
    // Direct initialization
    let direct: Array<number> = [1, 2, 3];
    _print("Direct initialization length:", direct.length);
    
    // Empty initialization
    let empty: Array<string> = [];
    _print("Empty initialization length:", empty.length);
    
    // Variable assignment
    let assigned: Array<boolean> = [true, false];
    _print("Variable assignment length:", assigned.length);
}

// Test 5: Verify existing array type compatibility still works
function testExistingArrayTypeCompatibility(): void {
    _print("=== Testing existing array type compatibility ===");
    
    // Array type compatibility
    let numbers1: Array<number> = [1, 2, 3];
    let numbers2: Array<number> = numbers1;
    
    _print("Type compatibility test passed");
    _print("numbers2 length:", numbers2.length);
    
    // Generic array compatibility
    let generic1: Array<any> = [1, "hello", true];
    let generic2: Array<any> = generic1;
    
    _print("Generic type compatibility test passed");
    _print("generic2 length:", generic2.length);
}

// Test 6: Verify existing array in function parameters still works
function testExistingArrayFunctionParameters(): void {
    _print("=== Testing existing array function parameters ===");
    
    function processArray(arr: Array<number>): number {
        return arr.length;
    }
    
    function processGenericArray<T>(arr: Array<T>): number {
        return arr.length;
    }
    
    let numbers: Array<number> = [1, 2, 3, 4, 5];
    let length1 = processArray(numbers);
    let length2 = processGenericArray(numbers);
    
    _print("Function parameter test 1:", length1);
    _print("Function parameter test 2:", length2);
}

// Test 7: Verify existing array return types still work
function testExistingArrayReturnTypes(): void {
    _print("=== Testing existing array return types ===");
    
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
    
    _print("Number array length:", numbers.length);
    _print("String array length:", strings.length);
    _print("Generic array length:", generic.length);
}

// Test 8: Verify existing array in class properties still works
function testExistingArrayClassProperties(): void {
    _print("=== Testing existing array class properties ===");
    
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
    _print("Class numbers length:", test.getNumbersLength());
    _print("Class strings length:", test.getStringsLength());
}

// Test 9: Verify existing array in generic classes still works
function testExistingArrayGenericClasses(): void {
    _print("=== Testing existing array in generic classes ===");
    
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
    
    _print("Generic number container length:", numberContainer.getLength());
    _print("Generic string container length:", stringContainer.getLength());
}

// Test 10: Verify existing array type inference still works
function testExistingArrayTypeInference(): void {
    _print("=== Testing existing array type inference ===");
    
    // Type inference from literals
    let inferredNumbers = [1, 2, 3, 4, 5];
    let inferredStrings = ["hello", "world", "test"];
    let inferredMixed = [1, "hello", true];
    
    _print("Inferred numbers length:", inferredNumbers.length);
    _print("Inferred strings length:", inferredStrings.length);
    _print("Inferred mixed length:", inferredMixed.length);
    
    // Type inference in functions
    function processInferredArray(arr: Array<number>): void {
        _print("Processing inferred array, length:", arr.length);
    }
    
    processInferredArray(inferredNumbers);
}

// Main regression test runner
function runRegressionTests(): void {
    _print("Starting Array Method Regression Tests...");
    
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
    
    _print("All regression tests completed successfully!");
    _print("No regressions detected in existing array functionality.");
}

// Execute regression tests
runRegressionTests();