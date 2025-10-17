// Comprehensive test for generic method resolution and LLVM IR generation
// This test validates the fixes we implemented for generic method calls

class GenericContainer<T> {
    private value: T;
    private name: string;

    constructor(val: T, name: string) {
        this.value = val;
        this.name = name;
    }

    getValue(): T {
        return this.value;
    }

    getName(): string {
        return this.name;
    }

    setValue(newValue: T): void {
        this.value = newValue;
    }

    toString(): string {
        return `GenericContainer<${this.name}>(${this.value})`;
    }
}

class GenericPair<A, B> {
    first: A;
    second: B;

    constructor(a: A, b: B) {
        this.first = a;
        this.second = b;
    }

    getFirst(): A {
        return this.first;
    }

    getSecond(): B {
        return this.second;
    }

    swap(): GenericPair<B, A> {
        return new GenericPair<B, A>(this.second, this.first);
    }
}

class GenericCalculator<T extends number> {
    private value: T;

    constructor(val: T) {
        this.value = val;
    }

    add(other: T): T {
        return (this.value + other) as T;
    }

    multiply(factor: T): T {
        return (this.value * factor) as T;
    }

    getValue(): T {
        return this.value;
    }
}

// Test cases for different generic scenarios
function testGenericMethods(): void {
    // Test 1: Single type parameter
    let numberContainer = new GenericContainer<number>(42, "number");
    let numberValue = numberContainer.getValue();
    let numberName = numberContainer.getName();
    
    _print("Number container value:", numberValue);
    _print("Number container name:", numberName);
    
    numberContainer.setValue(100);
    let newValue = numberContainer.getValue();
    _print("Updated number value:", newValue);
    
    // Test 2: String type parameter
    let stringContainer = new GenericContainer<string>("Hello", "string");
    let stringValue = stringContainer.getValue();
    let stringName = stringContainer.getName();
    
    _print("String container value:", stringValue);
    _print("String container name:", stringName);
    
    // Test 3: Multiple type parameters
    let pair = new GenericPair<number, string>(42, "test");
    let first = pair.getFirst();
    let second = pair.getSecond();
    
    _print("Pair first:", first);
    _print("Pair second:", second);
    
    // Test 4: Method chaining
    let swapped = pair.swap();
    let swappedFirst = swapped.getFirst();
    let swappedSecond = swapped.getSecond();
    
    _print("Swapped first:", swappedFirst);
    _print("Swapped second:", swappedSecond);
    
    // Test 5: Generic with constraints
    let calculator = new GenericCalculator<number>(10);
    let sum = calculator.add(5);
    let product = calculator.multiply(3);
    
    _print("Calculator value:", calculator.getValue());
    _print("Sum result:", sum);
    _print("Product result:", product);
    
    // Test 6: toString method
    let containerStr = numberContainer.toString();
    let pairStr = pair.toString();
    
    _print("Container string:", containerStr);
    _print("Pair string:", pairStr);
}

// Test nested generic calls
function testNestedGenericCalls(): void {
    let outer = new GenericContainer<GenericContainer<number>>(
        new GenericContainer<number>(42, "inner"), 
        "outer"
    );
    
    let inner = outer.getValue();
    let innerValue = inner.getValue();
    let innerName = inner.getName();
    
    _print("Nested inner value:", innerValue);
    _print("Nested inner name:", innerName);
    
    // Test method chaining on nested generics
    inner.setValue(100);
    let updatedInnerValue = inner.getValue();
    _print("Updated nested value:", updatedInnerValue);
}

// Test generic arrays
function testGenericArrays(): void {
    let containers: GenericContainer<number>[] = [
        new GenericContainer<number>(1, "first"),
        new GenericContainer<number>(2, "second"),
        new GenericContainer<number>(3, "third")
    ];
    
    for (let i = 0; i < containers.length; i++) {
        let container = containers[i];
        let value = container.getValue();
        let name = container.getName();
        
        _print("Array element", i, "value:", value, "name:", name);
    }
}

// Main test execution
function main(): void {
    _print("=== Generic Method Comprehensive Test ===");
    
    testGenericMethods();
    _print("---");
    testNestedGenericCalls();
    _print("---");
    testGenericArrays();
    
    _print("=== Test Complete ===");
}

main();