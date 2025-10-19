// Regression test for source location handling across different statement types
// This test exercises source location tracking for various TypeScript statement types

// Variable declarations with different types
let numberVar: number = 42;
let stringVar: string = "hello world";
let booleanVar: boolean = true;
let arrayVar: number[] = [1, 2, 3, 4, 5];
let objectVar = { 
    name: "test", 
    value: 100, 
    active: true 
};

// Function declarations with different parameter types
function simpleFunction(): void {
    console.log("Simple function called");
}

function parameterFunction(param1: number, param2: string): string {
    return param1.toString() + " " + param2;
}

function optionalParameterFunction(required: number, optional?: string): string {
    if (optional) {
        return required.toString() + " " + optional;
    }
    return required.toString();
}

// Arrow functions (if supported)
// let arrowFunction = (x: number): number => x * 2;

// Class declarations
class BaseClass {
    protected value: number;
    
    constructor(initValue: number) {
        this.value = initValue;
    }
    
    public getValue(): number {
        return this.value;
    }
    
    protected processValue(multiplier: number): number {
        return this.value * multiplier;
    }
}

class DerivedClass extends BaseClass {
    private name: string;
    
    constructor(initValue: number, initName: string) {
        super(initValue);
        this.name = initName;
    }
    
    public getName(): string {
        return this.name;
    }
    
    public getProcessedValue(multiplier: number): string {
        let processed: number = this.processValue(multiplier);
        return this.name + ": " + processed.toString();
    }
}

// Interface declarations
interface TestInterface {
    id: number;
    name: string;
    process(): string;
}

class InterfaceImplementation implements TestInterface {
    public id: number;
    public name: string;
    
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
    }
    
    public process(): string {
        return "ID: " + this.id.toString() + ", Name: " + this.name;
    }
}

// Enum declarations
enum Status {
    Pending = 0,
    InProgress = 1,
    Completed = 2,
    Failed = 3
}

enum StringEnum {
    First = "first",
    Second = "second",
    Third = "third"
}

// Type alias declarations
type UserID = number;
type UserName = string;
type User = {
    id: UserID;
    name: UserName;
    status: Status;
};

// Control flow statements
function controlFlowTest(value: number): string {
    // If-else statements
    if (value > 100) {
        return "High value: " + value.toString();
    } else if (value > 50) {
        return "Medium value: " + value.toString();
    } else {
        return "Low value: " + value.toString();
    }
}

function loopTest(maxIterations: number): number {
    let sum: number = 0;
    
    // While loop
    let i: number = 0;
    while (i < maxIterations) {
        sum += i;
        i++;
    }
    
    // For loop (if supported)
    // for (let j: number = 0; j < maxIterations; j++) {
    //     sum += j * 2;
    // }
    
    return sum;
}

// Switch statements
function switchTest(status: Status): string {
    switch (status) {
        case Status.Pending:
            return "Task is pending";
        case Status.InProgress:
            return "Task is in progress";
        case Status.Completed:
            return "Task is completed";
        case Status.Failed:
            return "Task failed";
        default:
            return "Unknown status";
    }
}

// Try-catch-finally statements (if supported)
// function exceptionTest(shouldThrow: boolean): string {
//     try {
//         if (shouldThrow) {
//             throw new Error("Test error");
//         }
//         return "No error occurred";
//     } catch (error) {
//         return "Caught error: " + error.message;
//     } finally {
//         console.log("Finally block executed");
//     }
// }

// Return statements
function returnTest(condition: boolean): number {
    if (condition) {
        return 42;
    }
    return 0;
}

// Expression statements
function expressionTest(): void {
    // Assignment expressions
    let x: number = 10;
    let y: number = 20;
    let z: number = x + y;
    
    // Function call expressions
    console.log("Expression test:", z);
    
    // Method call expressions
    let instance: DerivedClass = new DerivedClass(100, "test");
    let result: string = instance.getProcessedValue(2);
    console.log("Method result:", result);
    
    // Property access expressions
    let user: User = { id: 1, name: "John", status: Status.Pending };
    console.log("User name:", user.name);
    console.log("User status:", user.status);
    
    // Array access expressions
    let arrayElement: number = arrayVar[2];
    console.log("Array element:", arrayElement);
    
    // Object property access expressions
    let objectProperty: string = objectVar.name;
    console.log("Object property:", objectProperty);
}

// Module-level expressions
let globalExpression: number = numberVar * 2;
let globalStringExpression: string = stringVar + " processed";

// This test should compile successfully and exercise source location handling
// across all supported statement types in the TypeScript compiler
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()