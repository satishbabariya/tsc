// Phase 3 Complete - Semantic Analysis Demo
// This demonstrates the full semantic analysis capabilities

// ✅ Type inference from literals
let age = 25;                    // inferred as number
let name = "Alice";              // inferred as string  
let isActive = true;             // inferred as boolean

// ✅ Arithmetic operations with type checking
let nextYear = age + 1;          // number + number = number
let yearsLeft = 100 - age;       // number - number = number
let doubled = age * 2;           // number * number = number

// ✅ String operations with type checking
let greeting = "Hello, " + name; // string + string = string
let message = name + " is " + age; // string + string + number = string

// ✅ Boolean operations with type checking
let isAdult = age >= 18;         // number >= number = boolean
let canVote = isAdult && isActive; // boolean && boolean = boolean
let status = isActive || false;   // boolean || boolean = boolean

// ✅ Function declarations with parameter analysis
function calculateTax(income, rate) {
    let tax = income * rate;     // any * any = number (inferred)
    let afterTax = income - tax; // any - number = number
}

function greetUser(userName) {
    let personalGreeting = "Welcome, " + userName; // string + any = string
}

// ✅ Nested scopes and variable resolution
{
    let localVar = age * 2;      // Access outer scope variable
    let innerCalc = localVar + 10; // Local scope calculation
    
    {
        let deepNested = innerCalc - 5; // Access parent scope
    }
}

// ✅ Complex expressions with proper precedence
let complex = (age + 5) * 2 - 10; // Proper operator precedence
let condition = (age > 18) && (name != ""); // Mixed type operations

// ✅ Variable usage tracking (these will generate warnings)
let unused1 = 42;
let unused2 = "never used";

// ✅ Used variables (no warnings)
let result = complex + 10; // Use the complex calculation
let displayMessage = message + " - Result: " + result; // Use multiple variables