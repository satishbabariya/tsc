// Test variable declarations without type annotations
function testVariables() {
    // Let declarations
    let x = 10;
    let y = 20;
    let z;
    
    // Const declarations
    const PI = 3.14;
    const name = "test";
    
    // Var declarations
    var old = 100;
    var legacy = true;
    
    // Assignment
    z = "assigned";
    x = x + y;
    
    return x;
}