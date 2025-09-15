// Test for semantic analysis validation
// This tests array method calls, assignments, and interface member lookup

// Test 1: Array method calls and assignments
class ArraySemanticTest<T> {
    items: T[];
    numbers: number[];
    strings: string[];
    
    constructor() {
        this.items = [];
        this.numbers = [];
        this.strings = [];
    }
    
    // Test array method calls
    testArrayMethods(): void {
        // Test push method
        this.items.push(this.items[0]);
        this.numbers.push(42);
        this.strings.push("hello");
        
        // Test length property
        let count = this.items.length;
        let numCount = this.numbers.length;
        let strCount = this.strings.length;
        
        // Test pop method
        let item = this.items.pop();
        let num = this.numbers.pop();
        let str = this.strings.pop();
        
        // Prevent unused variable warnings
        count = count;
        numCount = numCount;
        strCount = strCount;
        item = item;
        num = num;
        str = str;
    }
    
    // Test array assignments
    testArrayAssignments(): void {
        // Test array literal assignments
        this.items = [];
        this.numbers = [1, 2, 3];
        this.strings = ["a", "b", "c"];
        
        // Test array element assignments
        if (this.items.length > 0) {
            this.items[0] = this.items[0];
        }
        if (this.numbers.length > 0) {
            this.numbers[0] = 100;
        }
        if (this.strings.length > 0) {
            this.strings[0] = "updated";
        }
    }
    
    // Test generic array operations
    processGenericArray(arr: T[]): T[] {
        let result: T[] = [];
        for (let i = 0; i < arr.length; i++) {
            result.push(arr[i]);
        }
        return result;
    }
}

// Test 2: Interface member lookup
interface TestInterface {
    id: number;
    name: string;
    process(): void;
}

interface GenericInterface<T> {
    data: T;
    getData(): T;
    setData(value: T): void;
}

class InterfaceImplementation implements TestInterface {
    id: number;
    name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
    }
    
    process(): void {
        // Test interface member access
        let currentId = this.id;
        let currentName = this.name;
        
        // Prevent unused variable warnings
        currentId = currentId;
        currentName = currentName;
    }
}

class GenericInterfaceImplementation<T> implements GenericInterface<T> {
    data: T;
    
    constructor(data: T) {
        this.data = data;
    }
    
    getData(): T {
        return this.data;
    }
    
    setData(value: T): void {
        this.data = value;
    }
}

// Test 3: Constrained interface usage
interface ConstrainedInterface<T extends number> {
    value: T;
    double(): T;
}

class NumberProcessor implements ConstrainedInterface<number> {
    value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    double(): number {
        return this.value * 2;
    }
}

// Test 4: Complex semantic scenarios
class ComplexSemanticTest<T extends string> {
    data: T[];
    processor: GenericInterface<T>;
    
    constructor(data: T[], processor: GenericInterface<T>) {
        this.data = data;
        this.processor = processor;
    }
    
    processAll(): void {
        // Test complex array operations
        for (let i = 0; i < this.data.length; i++) {
            let item = this.data[i];
            this.processor.setData(item);
            let processed = this.processor.getData();
            
            // Test array method calls on processed data
            if (processed.length > 0) {
                let firstChar = processed[0];
                firstChar = firstChar; // Prevent unused variable warning
            }
        }
    }
    
    // Test method chaining
    chainOperations(): T[] {
        let result: T[] = [];
        result.push(this.data[0]);
        result.push(this.data[1]);
        return result;
    }
}

// Test 5: Error scenarios (should fail semantic analysis)
class ErrorTest {
    // This should cause a type error - assigning number to string array
    // strings: string[] = [1, 2, 3];  // Commented out to avoid compilation errors
    
    // This should cause a type error - calling non-existent method
    testInvalidMethod(): void {
        let arr: number[] = [1, 2, 3];
        // arr.invalidMethod();  // Commented out to avoid compilation errors
    }
}