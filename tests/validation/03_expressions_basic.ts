// Test basic expressions
function testBasicExpressions() {
    let a = 10;
    let b = 5;
    let result;

    // Basic arithmetic
    result = a + b;
    result = a - b;
    result = a * b;
    result = a / b;

    // Basic comparison
    let comp1 = a > b;
    let comp2 = a < b;
    let comp3 = a == b;

    // Basic unary
    let neg = -a;
    let not = !true;

    // Basic assignment
    a = a + 5;

    return result;
}