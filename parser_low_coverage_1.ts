
// Edge cases and error conditions for parser coverage
function parserEdgeCases(): void {
    // Empty statements
    ;
    
    // Single character expressions
    let a = 1;
    let b = "a";
    let c = true;
    
    // Minimal expressions
    let d = a;
    let e = a + b;
    let f = a ? b : c;
    
    // Edge case operators
    let g = a++;
    let h = ++a;
    let i = a--;
    let j = --a;
    
    // Edge case comparisons
    let k = a == b;
    let l = a != b;
    let m = a === b;
    let n = a !== b;
    
    // Edge case logical operators
    let o = a && b;
    let p = a || b;
    let q = !a;
    
    // Edge case assignments
    a += 1;
    a -= 1;
    a *= 2;
    a /= 2;
    a %= 3;
    
    console.log(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q);
}
