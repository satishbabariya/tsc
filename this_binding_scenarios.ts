
// This binding scenarios testing
class BaseClass {
    protected value: number;
    
    constructor(value: number) {
        this.value = value;
    }
    
    getValue(): number {
        return this.value;
    }
    
    getValueArrow = (): number => {
        return this.value;
    }
}

class DerivedClass extends BaseClass {
    constructor(value: number) {
        super(value);
    }
    
    getValue(): number {
        return this.value * 2;
    }
    
    getValueArrow = (): number => {
        return this.value * 2;
    }
}

function testThisBindingScenarios() {
    const base = new BaseClass(10);
    const derived = new DerivedClass(10);
    
    // Test method calls
    console.log("Base getValue:", base.getValue());
    console.log("Derived getValue:", derived.getValue());
    
    // Test arrow function methods
    console.log("Base getValueArrow:", base.getValueArrow());
    console.log("Derived getValueArrow:", derived.getValueArrow());
    
    // Test method borrowing
    const borrowedMethod = base.getValue;
    const boundMethod = borrowedMethod.bind(derived);
    console.log("Borrowed method:", boundMethod());
    
    // Test method as callback
    function processCallback(callback: () => number): number {
        return callback();
    }
    
    console.log("Base callback:", processCallback(base.getValue.bind(base)));
    console.log("Derived callback:", processCallback(derived.getValue.bind(derived)));
}

// Test with object literals
function testObjectLiteralThisBinding() {
    const obj = {
        value: 42,
        
        // Regular method
        getValue(): number {
            return this.value;
        },
        
        // Arrow function method
        getValueArrow: (): number => {
            // this.value; // This would cause an error
            return 0;
        },
        
        // Method that returns arrow function
        getValueArrowMethod() {
            return () => {
                return this.value;
            };
        },
        
        // Method with destructuring
        getDestructuredValue() {
            const { value } = this;
            return value;
        }
    };
    
    console.log("Object getValue:", obj.getValue());
    console.log("Object getValueArrow:", obj.getValueArrow());
    console.log("Object getValueArrowMethod:", obj.getValueArrowMethod()());
    console.log("Object getDestructuredValue:", obj.getDestructuredValue());
    
    // Test method borrowing
    const borrowedGetValue = obj.getValue;
    const boundGetValue = borrowedGetValue.bind(obj);
    console.log("Borrowed getValue:", boundGetValue());
}

// Test with complex this binding
function testComplexThisBinding() {
    class Calculator {
        private result: number;
        
        constructor(initialValue: number = 0) {
            this.result = initialValue;
        }
        
        add(value: number): Calculator {
            this.result += value;
            return this;
        }
        
        multiply(value: number): Calculator {
            this.result *= value;
            return this;
        }
        
        getResult(): number {
            return this.result;
        }
        
        // Method that returns a function
        getAddFunction(): (value: number) => Calculator {
            return (value: number) => {
                this.result += value;
                return this;
            };
        }
        
        // Method with destructuring
        getDestructuredResult() {
            const { result } = this;
            return result;
        }
    }
    
    const calc = new Calculator(10);
    
    // Test method chaining
    const result = calc
        .add(5)
        .multiply(2)
        .add(3)
        .getResult();
    console.log("Calculator result:", result);
    
    // Test method that returns function
    const addFunction = calc.getAddFunction();
    const newResult = addFunction(10).getResult();
    console.log("New calculator result:", newResult);
    
    // Test destructured result
    const destructuredResult = calc.getDestructuredResult();
    console.log("Destructured result:", destructuredResult);
}

// Test with async methods
function testAsyncThisBinding() {
    class AsyncProcessor {
        private data: number[];
        
        constructor(data: number[]) {
            this.data = data;
        }
        
        async processData(): Promise<number[]> {
            return this.data.map(item => item * 2);
        }
        
        async processDataArrow = async (): Promise<number[]> => {
            return this.data.map(item => item * 2);
        }
        
        async processDataWithDestructuring(): Promise<number[]> {
            const { data } = this;
            return data.map(item => item * 2);
        }
    }
    
    const processor = new AsyncProcessor([1, 2, 3, 4, 5]);
    
    // Test async method
    processor.processData().then(result => {
        console.log("Async processData result:", result);
    });
    
    // Test async arrow method
    processor.processDataArrow().then(result => {
        console.log("Async processDataArrow result:", result);
    });
    
    // Test async method with destructuring
    processor.processDataWithDestructuring().then(result => {
        console.log("Async processDataWithDestructuring result:", result);
    });
}

// Run tests
testThisBindingScenarios();
testObjectLiteralThisBinding();
testComplexThisBinding();
testAsyncThisBinding();
