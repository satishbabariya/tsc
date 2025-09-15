// Basic Types and Variables
// Demonstrates all primitive types and variable declarations

// Number types
let integer: number = 42;
let float: number = 3.14159;
let negative: number = -100;

// String types
let singleQuote: string = 'Hello';
let doubleQuote: string = "World";
let template: string = `Value: ${integer}`;

// Boolean types
let isTrue: boolean = true;
let isFalse: boolean = false;

// Null and undefined
let nullValue: null = null;
let undefinedValue: undefined = undefined;

// Void type (for functions that don't return)
function logMessage(): void {
    // This function doesn't return anything
}

// Any type (when type is unknown)
let dynamicValue: any = "This can be anything";
dynamicValue = 42;
dynamicValue = true;

// Const declarations
const PI: number = 3.14159;
const APP_NAME: string = "TSC Compiler";

// Var declarations (function scoped)
var globalVar: number = 100;

// Type annotations
let explicitNumber: number = 42;
let explicitString: string = "TypeScript";
let explicitBoolean: boolean = true;

// Multiple variable declarations
let x: number = 10, y: number = 20, z: number = 30;

// Type inference (TypeScript can infer types)
let inferredNumber = 42;        // TypeScript infers: number
let inferredString = "Hello";   // TypeScript infers: string
let inferredBoolean = true;     // TypeScript infers: boolean

// This example demonstrates basic type system functionality