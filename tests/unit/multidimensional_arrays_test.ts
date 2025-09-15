// Test for multi-dimensional generic arrays
// This tests the core multi-dimensional array functionality

// Test 1: Basic multi-dimensional arrays
class MultiDimContainer<T> {
    // 2D arrays
    matrix: T[][];
    grid: T[][];
    
    // 3D arrays
    cube: T[][][];
    
    // Methods working with multi-dimensional arrays
    getMatrixElement(row: number, col: number): T {
        return this.matrix[row][col];
    }
    
    setMatrixElement(row: number, col: number, value: T): void {
        this.matrix[row][col] = value;
    }
    
    getCubeElement(x: number, y: number, z: number): T {
        return this.cube[x][y][z];
    }
    
    getDimensions(): number {
        return this.matrix.length;
    }
}

// Test 2: Arrays of generic objects
class GenericObjectContainer<T> {
    // Array of generic objects
    objects: T[];
    objectList: T[];
    
    // Multi-dimensional arrays of generic objects
    objectMatrix: T[][];
    
    // Methods working with generic object arrays
    addObject(obj: T): void {
        this.objects.push(obj);
    }
    
    getObject(index: number): T {
        return this.objects[index];
    }
    
    getObjectCount(): number {
        return this.objects.length;
    }
}

// Test 3: Array methods on multi-dimensional arrays
class ArrayMethodsContainer<T> {
    data: T[];
    matrix: T[][];
    
    // Test array methods on generic arrays
    pushValue(value: T): number {
        return this.data.push(value);
    }
    
    popValue(): T {
        return this.data.pop();
    }
    
    getLength(): number {
        return this.data.length;
    }
    
    // Test array methods on multi-dimensional arrays
    pushRow(row: T[]): number {
        return this.matrix.push(row);
    }
    
    popRow(): T[] {
        return this.matrix.pop();
    }
    
    getRowCount(): number {
        return this.matrix.length;
    }
    
    // Test chaining
    addAndGetLength(value: T): number {
        this.data.push(value);
        return this.data.length;
    }
}

// Test 4: Generic arrays as parameters and return types
class GenericArrayFunctions<T> {
    // Generic array as parameter
    processArray(arr: T[]): void {
        let count = arr.length;
        count = count; // Prevent unused variable warning
    }
    
    // Multi-dimensional array as parameter
    processMatrix(matrix: T[][]): void {
        let rowCount = matrix.length;
        rowCount = rowCount; // Prevent unused variable warning
    }
    
    // Generic array as return type
    createArray(): T[] {
        return [];
    }
    
    // Multi-dimensional array as return type
    createMatrix(): T[][] {
        return [];
    }
}

// Test 5: Arrays with constrained generics
class ConstrainedArrayContainer<T extends string> {
    stringArray: T[];
    stringMatrix: T[][];
    
    addString(str: T): void {
        this.stringArray.push(str);
    }
    
    getString(index: number): T {
        return this.stringArray[index];
    }
    
    getStringCount(): number {
        return this.stringArray.length;
    }
}

// Test 6: Nested arrays with interface constraints
interface BaseInterface {
    id: number;
}

class InterfaceArrayContainer<T extends BaseInterface> {
    interfaceArray: T[];
    interfaceMatrix: T[][];
    
    addInterface(obj: T): void {
        this.interfaceArray.push(obj);
    }
    
    getInterface(index: number): T {
        return this.interfaceArray[index];
    }
    
    getInterfaceCount(): number {
        return this.interfaceArray.length;
    }
}