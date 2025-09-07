// Test function declarations and calls
function add(a, b) {
    return a + b;
}

function greet(name) {
    return "Hello " + name;
}

function noReturn() {
    let x = 42;
}

function testFunctions() {
    // Function calls
    let sum = add(10, 5);
    let message = greet("World");
    
    // Call function without return
    noReturn();
    
    return sum;
}