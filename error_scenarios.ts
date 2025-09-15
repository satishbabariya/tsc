
// Error scenarios testing
function testErrorScenarios() {
    // Test with null/undefined
    const nullValue = null;
    const undefinedValue = undefined;
    
    // These should cause errors
    // const { prop } = nullValue; // Error: Cannot destructure property 'prop' of 'null'
    // const { prop } = undefinedValue; // Error: Cannot destructure property 'prop' of 'undefined'
    
    // Safe destructuring with defaults
    const { prop = "default" } = nullValue || {};
    console.log("Safe destructuring:", prop);
    
    // Test with non-object types
    const stringValue = "hello";
    const numberValue = 42;
    
    // These should cause errors
    // const { prop } = stringValue; // Error: Cannot destructure property 'prop' of 'string'
    // const { prop } = numberValue; // Error: Cannot destructure property 'prop' of 'number'
    
    // Test with missing properties
    const incompleteObject = { name: "Alice" };
    
    // Destructuring non-existent properties
    const { name, age = 0, city = "Unknown" } = incompleteObject;
    console.log("Name:", name);
    console.log("Age (default):", age);
    console.log("City (default):", city);
}

// Test with type mismatches
function testTypeMismatches() {
    const user = {
        name: "Alice",
        age: 30,
        isActive: true
    };
    
    // These should cause type errors
    // const { name }: number = user; // Error: Type 'string' is not assignable to type 'number'
    // const { age }: string = user; // Error: Type 'number' is not assignable to type 'string'
    
    // Correct destructuring with types
    const { name }: { name: string } = user;
    const { age }: { age: number } = user;
    const { isActive }: { isActive: boolean } = user;
    
    console.log("Name:", name);
    console.log("Age:", age);
    console.log("Is active:", isActive);
}

// Test with invalid destructuring patterns
function testInvalidPatterns() {
    const array = [1, 2, 3, 4, 5];
    
    // These should cause errors
    // const { prop } = array; // Error: Cannot destructure property 'prop' of 'number[]'
    // const [a, b, c] = { prop: "value" }; // Error: Object is not iterable
    
    // Correct patterns
    const [first, second, third] = array;
    console.log("First:", first);
    console.log("Second:", second);
    console.log("Third:", third);
    
    const obj = { prop: "value" };
    const { prop } = obj;
    console.log("Prop:", prop);
}

// Test with reserved keywords
function testReservedKeywords() {
    const data = {
        class: "test-class",
        function: "test-function",
        const: "test-const"
    };
    
    // These should cause errors
    // const { class } = data; // Error: 'class' is a reserved keyword
    // const { function } = data; // Error: 'function' is a reserved keyword
    // const { const } = data; // Error: 'const' is a reserved keyword
    
    // Correct way with renaming
    const { class: className, function: functionName, const: constName } = data;
    console.log("Class name:", className);
    console.log("Function name:", functionName);
    console.log("Const name:", constName);
}

// Test with circular references
function testCircularReferences() {
    const obj1: any = { name: "Object 1" };
    const obj2: any = { name: "Object 2" };
    
    obj1.ref = obj2;
    obj2.ref = obj1;
    
    // Destructuring should work fine
    const { name: name1, ref: { name: name2 } } = obj1;
    console.log("Name 1:", name1);
    console.log("Name 2:", name2);
}

// Run tests
testErrorScenarios();
testTypeMismatches();
testInvalidPatterns();
testReservedKeywords();
testCircularReferences();
