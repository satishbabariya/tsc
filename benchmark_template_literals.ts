// Template Literal Performance Benchmarks

// Test 1: Basic template literals (baseline)
let basic1 = `hello`;
let basic2 = `hello world`;
let basic3 = `hello\nworld`;
let basic4 = `hello 世界`;

// Test 2: Simple interpolation
let name = "World";
let simple1 = `${name}`;
let simple2 = `Hello ${name}`;
let simple3 = `${name}!`;
let simple4 = `Hello ${name}!`;

// Test 3: Multiple interpolations
let a = "A";
let b = "B";
let c = "C";
let multiple1 = `${a}${b}`;
let multiple2 = `${a}${b}${c}`;
let multiple3 = `Hello ${name}, ${a}!`;
let multiple4 = `Hello ${name}, you are ${a} years old!`;

// Test 4: Long template literals
let long1 = `This is a very long template literal with multiple parts and interpolations like ${name} and more text to test the lexer and parser handling of long strings`;
let long2 = `Another long template literal with ${a} and ${b} and ${c} and more text to test performance with multiple interpolations in a single template literal`;

// Test 5: Mixed complexity
let mixed1 = `Hello ${name}!`;
let mixed2 = `Hello ${name}, ${a}!`;
let mixed3 = `Hello ${name}, you are ${a} years old!`;
let mixed4 = `Hello ${name}, you are ${a} years old and your favorite letter is ${b}!`;