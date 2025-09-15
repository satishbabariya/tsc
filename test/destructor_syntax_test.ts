// Test file for basic destructor syntax parsing
class TestClass {
    private resource: string;
    
    constructor(resource: string) {
        this.resource = resource;
    }
    
    ~TestClass() {
        console.log("Cleaning up resource:", this.resource);
    }
}

class FileHandle {
    private filename: string;
    
    constructor(filename: string) {
        this.filename = filename;
    }
    
    ~FileHandle() {
        console.log("Closing file:", this.filename);
    }
}

// Test class with no destructor
class SimpleClass {
    value: number = 42;
}

// Test class with both constructor and destructor
class ComplexClass {
    private data: string[];
    
    constructor() {
        this.data = [];
    }
    
    addData(item: string): void {
        this.data.push(item);
    }
    
    ~ComplexClass() {
        console.log("Cleaning up", this.data.length, "items");
        this.data = [];
    }
}