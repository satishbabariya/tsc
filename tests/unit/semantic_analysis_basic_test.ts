// Basic semantic analysis validation test
// Focus on core array operations and type checking

// Test 1: Basic array operations
class ArrayTest {
    numbers: number[];
    strings: string[];
    
    constructor() {
        this.numbers = [];
        this.strings = [];
    }
    
    addNumber(value: number): void {
        this.numbers.push(value);
    }
    
    addString(value: string): void {
        this.strings.push(value);
    }
    
    getLength(): number {
        return this.numbers.length;
    }
    
    getStringLength(): number {
        return this.strings.length;
    }
}

// Test 2: Generic array operations
class GenericArrayTest<T> {
    items: T[];
    
    constructor() {
        this.items = [];
    }
    
    addItem(item: T): void {
        this.items.push(item);
    }
    
    getItemCount(): number {
        return this.items.length;
    }
}

// Test 3: Array literal assignments
class ArrayLiteralTest {
    numbers: number[];
    strings: string[];
    
    constructor() {
        this.numbers = [1, 2, 3];
        this.strings = ["a", "b", "c"];
    }
    
    updateNumbers(): void {
        this.numbers = [4, 5, 6];
    }
    
    updateStrings(): void {
        this.strings = ["x", "y", "z"];
    }
}

// Test 4: Generic function with arrays
function processArray<T>(items: T[]): number {
    return items.length;
}

function createNumberArray(): number[] {
    return [1, 2, 3, 4, 5];
}

function createStringArray(): string[] {
    return ["hello", "world"];
}

// Test 5: Array method chaining
class ArrayMethodTest {
    data: number[];
    
    constructor() {
        this.data = [];
    }
    
    addData(value: number): ArrayMethodTest {
        this.data.push(value);
        return this;
    }
    
    getDataLength(): number {
        return this.data.length;
    }
}

// Test 6: Multi-dimensional arrays
class MultiDimTest {
    matrix: number[][];
    
    constructor() {
        this.matrix = [[1, 2], [3, 4]];
    }
    
    getMatrixSize(): number {
        return this.matrix.length;
    }
    
    getRowSize(row: number): number {
        return this.matrix[row].length;
    }
}