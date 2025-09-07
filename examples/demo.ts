// TSC Compiler Demo - TypeScript syntax for native compilation

// Type declarations
interface Person {
    name: string;
    age: number;
    isActive?: boolean;
}

// Function with type annotations
function greet(person: Person): string {
    if (person.isActive) {
        return "Hello, " + person.name + "! You are " + person.age + " years old.";
    } else {
        return "Hello, " + person.name + "!";
    }
}

// Class definition with access modifiers
class Calculator {
    private value: number = 0;
    
    constructor(initialValue: number = 0) {
        this.value = initialValue;
    }
    
    public add(x: number): Calculator {
        this.value += x;
        return this;
    }
    
    public multiply(x: number): Calculator {
        this.value *= x;
        return this;
    }
    
    public getValue(): number {
        return this.value;
    }
}

// Generic function
function identity<T>(arg: T): T {
    return arg;
}

// Union types and type guards
type StringOrNumber = string | number;

function processValue(value: StringOrNumber): string {
    if (typeof value === "string") {
        return value.toUpperCase();
    } else {
        return value.toString();
    }
}

// Async function (for future coroutine support)
async function fetchData(): Promise<string> {
    // This will be compiled to LLVM coroutines
    const result = await someAsyncOperation();
    return result;
}

// Module exports
export { Person, Calculator, greet, identity, processValue };

// Main execution
function main(): number {
    const person: Person = {
        name: "TypeScript Developer",
        age: 30,
        isActive: true
    };
    
    console.log(greet(person));
    
    const calc = new Calculator(10)
        .add(5)
        .multiply(2);
    
    console.log("Result: " + calc.getValue());
    
    return 0;
}

// Entry point for native compilation
main();