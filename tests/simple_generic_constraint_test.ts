// Simple test for generic constraints
function testGeneric<T extends number>(value: T): T {
    return value;
}

_print("Generic constraint test completed");