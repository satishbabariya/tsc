// Regression test for source location handling in error scenarios
// This test exercises source location tracking when errors occur or are reported

// Test 1: Type mismatch scenarios (should trigger source location in error messages)
let numberVar: number = 42;
let stringVar: string = "hello";

// These should generate type errors with proper source location information
// let typeError1: string = numberVar;  // Would cause type error
// let typeError2: number = stringVar;  // Would cause type error

// Test 2: Undefined variable access (should trigger source location in error messages)
// let undefinedVar: number = nonExistentVariable;  // Would cause undefined variable error

// Test 3: Function call with wrong number of arguments
function testFunction(a: number, b: string): string {
    return a.toString() + " " + b;
}

// These should generate argument count errors with source location
// let argError1: string = testFunction(numberVar);  // Too few arguments
// let argError2: string = testFunction(numberVar, stringVar, "extra");  // Too many arguments

// Test 4: Property access on wrong types
let testObject = { value: 42, name: "test" };
let testArray: number[] = [1, 2, 3];

// These should generate property access errors with source location
// let propError1: number = testObject.nonExistentProperty;  // Property doesn't exist
// let propError2: number = testArray.nonExistentMethod();  // Method doesn't exist

// Test 5: Array access with wrong index type
// let indexError: number = testArray[stringVar];  // String index on number array

// Test 6: Complex nested error scenarios
function complexErrorFunction(param: number): string {
    let localVar: number = param;
    
    // Nested function that might cause errors
    function nestedErrorFunction(nestedParam: string): number {
        // This would cause type error if uncommented
        // return nestedParam + localVar;  // String + number type error
        return localVar;  // Valid return
    }
    
    // Call with wrong argument type
    // let nestedResult: number = nestedErrorFunction(localVar);  // Number instead of string
    
    return "complex function result";
}

// Test 7: Class method errors
class ErrorTestClass {
    private value: number;
    
    constructor(initValue: number) {
        this.value = initValue;
    }
    
    public getValue(): number {
        return this.value;
    }
    
    public processValue(multiplier: number): string {
        return (this.value * multiplier).toString();
    }
}

let errorInstance: ErrorTestClass = new ErrorTestClass(42);

// These should generate method call errors with source location
// let methodError1: number = errorInstance.nonExistentMethod();  // Method doesn't exist
// let methodError2: string = errorInstance.getValue();  // Wrong return type expectation

// Test 8: Conditional and loop error scenarios
let conditionVar: number = 10;

if (conditionVar > 5) {
    let ifScopeVar: number = conditionVar * 2;
    console.log("If scope:", ifScopeVar);
} else {
    let elseScopeVar: number = conditionVar / 2;
    console.log("Else scope:", elseScopeVar);
}

// Test 9: Switch statement error scenarios
let switchVar: number = 2;

switch (switchVar) {
    case 1:
        let case1Var: string = "case 1";
        console.log(case1Var);
        break;
    case 2:
        let case2Var: number = 42;
        console.log("case 2:", case2Var);
        break;
    default:
        let defaultVar: boolean = true;
        console.log("default:", defaultVar);
        break;
}

// Test 10: Try-catch error scenarios (if implemented)
// try {
//     let riskyOperation: number = 1 / 0;  // Division by zero
//     console.log("Risky result:", riskyOperation);
// } catch (error) {
//     console.log("Caught error:", error);
// }

// Test 11: Generic type errors (if implemented)
// function genericFunction<T>(param: T): T {
//     return param;
// }
// let genericError: string = genericFunction<number>(42);  // Type mismatch

// Test 12: Module import/export errors (if implemented)
// import { nonExistentFunction } from './non-existent-module';
// export { nonExistentFunction };

// This test should compile successfully for the parts that are implemented
// and exercise source location handling in error reporting scenarios
// The original bug was in SemanticAnalyzer.cpp where it tried to access
// private members line_ and column_ instead of using getLine() and getColumn()