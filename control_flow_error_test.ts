
// Control Flow Error Test Case
function controlFlowErrorTest(): void {
    // Malformed if statements
    if (true) {
        console.log("valid if");
    } else {
        console.log("valid else");
    }
    
    if (true) {
        console.log("missing closing brace");
    
    // Malformed loops
    for (let i = 0; i < 10; i++) {
        console.log(i);
    
    while (true) {
        console.log("infinite loop");
    
    // Malformed switch statements
    let value = 1;
    switch (value) {
        case 1:
            console.log("one");
        case 2:
            console.log("two");
        default:
            console.log("default");
    }
    
    // Break/continue outside loops
    function invalidBreak(): void {
        break;
    }
    
    function invalidContinue(): void {
        continue;
    }
}
