// Simple test for generic constraints
function testGeneric<T extends number>(value: T): T {
    return value;
}

console.log("Generic constraint test completed");