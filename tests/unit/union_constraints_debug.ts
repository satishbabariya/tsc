// Debug test to isolate union type constraint parsing issues

// Test 1: Simple union constraint in function (should work)
function testSimple<T extends string>(value: T): T {
    return value;
}

// Test 2: Union constraint in function (this fails)
function testUnion<T extends string | number>(value: T): T {
    return value;
}

// Test 3: Complex union constraint in function (this fails)
function testComplex<T extends string | number | boolean>(value: T): T {
    return value;
}

// Test 4: Union constraint with array types (this fails)
function testArrayUnion<T extends string[] | number[]>(value: T): T {
    return value;
}

// Test 5: Multiple union constraints (this fails)
function testMulti<T extends string | number, U extends boolean | null>(value: T, flag: U): T {
    return value;
}