function testAssignment(): number {
    let counter: number = 0;
    counter = counter + 1;
    return counter;
}

function main(): number {
    return testAssignment();
}