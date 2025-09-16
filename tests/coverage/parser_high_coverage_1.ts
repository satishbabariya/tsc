// Comprehensive expression parsing for high coverage
function comprehensiveExpressions(): void {
    // Arithmetic expressions
    let a = 1 + 2 - 3 * 4 / 5;
    let b = (1 + 2) * (3 - 4) / (5 + 6);
    let c = Math.sqrt(a) + Math.sin(b) * Math.cos(c);

    // Comparison expressions
    let d = a > b && b < c || c >= d;
    let e = a == b && b != c && c === d;
    let f = a <= b || b >= c && c !== d;

    // Logical expressions
    let g = true && false || !true;
    let h = (a > 0) ? "positive" : "negative";
    let i = a && b || c ? d : e;

    // String expressions
    let j = "hello" + " " + "world";
    let k = `Template ${a} literal ${b}`;
    let l = "String with \"quotes\" and 'apostrophes'";

    // Array expressions
    let m = [1, 2, 3, 4, 5];
    let n = [a, b, c, d, e];
    let o = m.concat(n).slice(1, 5);

    // Object expressions
    let p = {name: "test", value: 42, nested: {deep: true}};
    let q = {...p, additional: "property"};
    let r = {
        [a]: b, [c]: d, method() {
            return this.value;
        }
    };

    console.log(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r);
}
