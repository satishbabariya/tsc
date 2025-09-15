// Regression tests for DiagnosticEngine infinite recursion fix

// Test 1: Multiple syntax errors to trigger error limit
interface BadInterface1 {
    property: string
    // Missing semicolon
}

interface BadInterface2 {
    property: number
    // Missing semicolon
}

interface BadInterface3 {
    property: boolean
    // Missing semicolon
}

interface BadInterface4 {
    property: string
    // Missing semicolon
}

interface BadInterface5 {
    property: number
    // Missing semicolon
}

// Test 2: Complex syntax errors with generics
interface BadGeneric1<T extends string> {
    value: T
    // Missing semicolon
}

interface BadGeneric2<T extends number> {
    value: T
    // Missing semicolon
}

// Test 3: Function syntax errors
function badFunction1(param: string) {
    // Missing return type and body
}

function badFunction2(param: number) {
    // Missing return type and body
}

// Test 4: Variable declaration errors
let badVar1: string = "test"
let badVar2: number = 42
// Missing semicolons

// Test 5: Complex nested errors
interface OuterBad {
    inner: InnerBad
    // Missing semicolon
}

interface InnerBad {
    value: string
    // Missing semicolon
}