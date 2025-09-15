// Test all control flow statements
function testControlFlow() {
    let x = 10;
    let result = 0;
    
    // If/else statements
    if (x > 5) {
        result = 1;
    } else {
        result = 0;
    }
    
    // Nested if
    if (x > 0) {
        if (x < 20) {
            result = 2;
        }
    }
    
    // While loop
    let i = 0;
    while (i < 3) {
        result = result + 1;
        i = i + 1;
    }
    
    // Do-while loop
    let j = 0;
    do {
        j = j + 1;
    } while (j < 2);
    
    // For loop
    for (let k = 0; k < 2; k = k + 1) {
        result = result * 2;
    }
    
    // Switch statement
    switch (x) {
        case 10:
            result = 100;
            break;
        case 20:
            result = 200;
            break;
        default:
            result = 0;
            break;
    }
    
    return result;
}