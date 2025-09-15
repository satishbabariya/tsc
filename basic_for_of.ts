
// Basic for...of loop testing
function testBasicForOf() {
    const numbers = [1, 2, 3, 4, 5];
    const results: number[] = [];
    
    // Basic for...of loop
    for (const num of numbers) {
        results.push(num * 2);
    }
    
    console.log("Doubled numbers:", results);
    
    // For...of with let
    const squares: number[] = [];
    for (let num of numbers) {
        squares.push(num * num);
    }
    
    console.log("Squares:", squares);
    
    // For...of with var (legacy)
    const cubes: number[] = [];
    for (var num of numbers) {
        cubes.push(num * num * num);
    }
    
    console.log("Cubes:", cubes);
}

// Test with different data types
function testDifferentTypes() {
    const strings = ["hello", "world", "typescript"];
    const lengths: number[] = [];
    
    for (const str of strings) {
        lengths.push(str.length);
    }
    
    console.log("String lengths:", lengths);
    
    const booleans = [true, false, true, false];
    const negated: boolean[] = [];
    
    for (const bool of booleans) {
        negated.push(!bool);
    }
    
    console.log("Negated booleans:", negated);
}

// Test with mixed arrays
function testMixedArrays() {
    const mixed = [1, "hello", true, 42, "world"];
    const types: string[] = [];
    
    for (const item of mixed) {
        types.push(typeof item);
    }
    
    console.log("Item types:", types);
}

// Run tests
testBasicForOf();
testDifferentTypes();
testMixedArrays();
