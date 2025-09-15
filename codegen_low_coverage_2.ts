
// Complex control flow for code generation coverage
function complexControlFlow(): void {
    // Complex nested loops with breaks and continues
    outerLoop: for (let i = 0; i < 10; i++) {
        innerLoop: for (let j = 0; j < 10; j++) {
            if (i + j > 15) {
                break outerLoop;
            }
            if (i + j < 5) {
                continue innerLoop;
            }
            console.log(i, j);
        }
    }
    
    // Complex switch with fallthrough
    let value = 1;
    switch (value) {
        case 1:
            console.log("one");
            // fallthrough
        case 2:
            console.log("two");
            break;
        case 3:
            console.log("three");
            break;
        default:
            console.log("default");
    }
    
    // Complex try-catch with multiple catch blocks
    try {
        throw new Error("test error");
    } catch (error) {
        console.log("caught error:", error);
    } finally {
        console.log("finally executed");
    }
    
    // Complex conditional expressions
    let result = (true ? (false ? "nested true" : "nested false") : "outer false");
    console.log(result);
}
