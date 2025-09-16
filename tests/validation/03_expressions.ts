// Test all expression types
function testExpressions() {
    let a = 10;
    let b = 5;
    let result;

    // Binary expressions - arithmetic
    result = a + b;      // 15
    result = a - b;      // 5
    result = a * b;      // 50
    result = a / b;      // 2
    result = a % b;      // 0

    // Binary expressions - comparison
    let comp1 = a > b;   // true
    let comp2 = a < b;   // false
    let comp3 = a >= b;  // true
    let comp4 = a <= b;  // false
    let comp5 = a == b;  // false
    let comp6 = a != b;  // true

    // Binary expressions - logical
    let log1 = true && false;  // false
    let log2 = true || false;  // true

    // Unary expressions
    let neg = -a;        // -10
    let pos = +b;        // 5
    let not = !true;     // false

    // Assignment expressions
    a += 5;              // a = 15
    b -= 2;              // b = 3
    result *= 2;         // result = 6

    return result;
}