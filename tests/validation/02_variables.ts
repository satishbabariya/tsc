// Test all variable declaration types
function testVariables() {
    // Let declarations
    let x = 10;
    let y: number = 20;
    let z: string;
    
    // Const declarations
    const PI = 3.14;
    const name: string = "test";
    
    // Var declarations
    var old = 100;
    var legacy: boolean = true;
    
    // Assignment
    z = "assigned";
    x = x + y;
    
    return x;
}