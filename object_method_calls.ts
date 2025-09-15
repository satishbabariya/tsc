
// Object method calls testing
class Calculator {
    private value: number;
    
    constructor(initialValue: number = 0) {
        this.value = initialValue;
    }
    
    add(num: number): Calculator {
        this.value += num;
        return this;
    }
    
    subtract(num: number): Calculator {
        this.value -= num;
        return this;
    }
    
    multiply(num: number): Calculator {
        this.value *= num;
        return this;
    }
    
    divide(num: number): Calculator {
        if (num !== 0) {
            this.value /= num;
        }
        return this;
    }
    
    getValue(): number {
        return this.value;
    }
    
    reset(): Calculator {
        this.value = 0;
        return this;
    }
}

function testObjectMethodCalls() {
    const calc = new Calculator(10);
    
    // Method chaining
    const result = calc
        .add(5)
        .multiply(2)
        .subtract(3)
        .divide(2)
        .getValue();
    
    console.log("Calculation result:", result);
    
    // Individual method calls
    calc.reset();
    calc.add(10);
    console.log("After add:", calc.getValue());
    
    calc.multiply(3);
    console.log("After multiply:", calc.getValue());
    
    calc.subtract(5);
    console.log("After subtract:", calc.getValue());
}

// Test with object literal methods
function testObjectLiteralMethods() {
    const mathUtils = {
        add(a: number, b: number): number {
            return a + b;
        },
        
        subtract(a: number, b: number): number {
            return a - b;
        },
        
        multiply(a: number, b: number): number {
            return a * b;
        },
        
        divide(a: number, b: number): number {
            return b !== 0 ? a / b : 0;
        },
        
        power(base: number, exponent: number): number {
            return Math.pow(base, exponent);
        },
        
        sqrt(num: number): number {
            return Math.sqrt(num);
        }
    };
    
    console.log("Add:", mathUtils.add(5, 3));
    console.log("Subtract:", mathUtils.subtract(5, 3));
    console.log("Multiply:", mathUtils.multiply(5, 3));
    console.log("Divide:", mathUtils.divide(15, 3));
    console.log("Power:", mathUtils.power(2, 3));
    console.log("Square root:", mathUtils.sqrt(16));
}

// Test with this binding
function testThisBinding() {
    const person = {
        name: "Alice",
        age: 30,
        
        greet(): string {
            return `Hello, I'm ${this.name} and I'm ${this.age} years old`;
        },
        
        celebrateBirthday(): void {
            this.age++;
        },
        
        introduce(): string {
            return this.greet();
        }
    };
    
    console.log("Greeting:", person.greet());
    console.log("Introduction:", person.introduce());
    
    person.celebrateBirthday();
    console.log("After birthday:", person.greet());
}

// Test with method borrowing
function testMethodBorrowing() {
    const obj1 = {
        name: "Object 1",
        getName(): string {
            return this.name;
        }
    };
    
    const obj2 = {
        name: "Object 2"
    };
    
    // Borrow method from obj1
    const getName = obj1.getName;
    const boundGetName = getName.bind(obj2);
    
    console.log("Obj1 name:", obj1.getName());
    console.log("Obj2 name (borrowed):", boundGetName());
}

// Test with arrow functions and this
function testArrowFunctionThis() {
    const counter = {
        count: 0,
        
        // Regular method
        increment(): void {
            this.count++;
        },
        
        // Arrow function method (preserves this from outer scope)
        incrementArrow: (): void => {
            // this.count++; // This would cause an error
        },
        
        // Method that returns arrow function
        getIncrementer() {
            return () => {
                this.count++;
            };
        },
        
        getCount(): number {
            return this.count;
        }
    };
    
    counter.increment();
    console.log("Count after increment:", counter.getCount());
    
    const incrementer = counter.getIncrementer();
    incrementer();
    console.log("Count after arrow incrementer:", counter.getCount());
}

// Run tests
testObjectMethodCalls();
testObjectLiteralMethods();
testThisBinding();
testMethodBorrowing();
testArrowFunctionThis();
