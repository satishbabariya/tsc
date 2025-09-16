// Comprehensive statement parsing for high coverage
function comprehensiveStatements(): void {
    // Variable declarations
    let x: number = 42;
    const y: string = "hello";
    var z: boolean = true;

    // If statements
    if (x > 0) {
        console.log("positive");
    } else if (x < 0) {
        console.log("negative");
    } else {
        console.log("zero");
    }

    // Switch statements
    switch (x) {
        case 1:
            console.log("one");
            break;
        case 2:
            console.log("two");
            break;
        default:
            console.log("other");
    }

    // For loops
    for (let i = 0; i < 10; i++) {
        console.log(i);
    }

    for (let item of [1, 2, 3]) {
        console.log(item);
    }

    for (let key in {a: 1, b: 2}) {
        console.log(key);
    }

    // While loops
    let j = 0;
    while (j < 5) {
        console.log(j);
        j++;
    }

    // Do-while loops
    let k = 0;
    do {
        console.log(k);
        k++;
    } while (k < 3);

    // Try-catch blocks
    try {
        throw new Error("test error");
    } catch (error) {
        console.log("caught error:", error);
    } finally {
        console.log("finally block");
    }

    // Return statements
    return "comprehensive statements";
}
