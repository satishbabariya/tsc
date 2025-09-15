
// Error reporting testing
function testErrorReporting() {
    // Test type mismatch errors
    const numbers: number[] = [1, 2, 3, 4, 5];
    
    // These should cause type errors
    // const [first]: string = numbers; // Error: Type 'number' is not assignable to type 'string'
    // const [first, second]: [string, string] = numbers; // Error: Type 'number' is not assignable to type 'string'
    
    // Test object type mismatch errors
    const user = {
        name: "Alice",
        age: 30,
        city: "New York"
    };
    
    // These should cause type errors
    // const { name }: { name: number } = user; // Error: Type 'string' is not assignable to type 'number'
    // const { age }: { age: string } = user; // Error: Type 'number' is not assignable to type 'string'
    
    // Test missing property errors
    const incompleteUser = { name: "Bob" };
    
    // These should cause type errors
    // const { name, age }: { name: string; age: number } = incompleteUser; // Error: Property 'age' is missing
    // const { name, age, city }: { name: string; age: number; city: string } = incompleteUser; // Error: Properties 'age' and 'city' are missing
    
    // Test invalid destructuring patterns
    const array = [1, 2, 3, 4, 5];
    const object = { prop: "value" };
    
    // These should cause errors
    // const { prop } = array; // Error: Cannot destructure property 'prop' of 'number[]'
    // const [a, b, c] = object; // Error: Object is not iterable
    
    // Test reserved keyword errors
    const data = {
        class: "test-class",
        function: "test-function",
        const: "test-const"
    };
    
    // These should cause errors
    // const { class } = data; // Error: 'class' is a reserved keyword
    // const { function } = data; // Error: 'function' is a reserved keyword
    // const { const } = data; // Error: 'const' is a reserved keyword
    
    // Test null/undefined errors
    const nullValue = null;
    const undefinedValue = undefined;
    
    // These should cause errors
    // const { prop } = nullValue; // Error: Cannot destructure property 'prop' of 'null'
    // const { prop } = undefinedValue; // Error: Cannot destructure property 'prop' of 'undefined'
    
    // Test invalid property access
    const obj = { name: "Alice" };
    
    // These should cause errors
    // const { nonExistentProp } = obj; // Error: Property 'nonExistentProp' does not exist
    // const { name: reservedKeyword } = obj; // Error: 'reservedKeyword' is a reserved keyword
}

// Test with function parameter errors
function testFunctionParameterErrors() {
    // These should cause type errors
    // function processArray([first, second]: [string, string]): void {
    //     console.log(first, second);
    // }
    // processArray([1, 2]); // Error: Type 'number' is not assignable to type 'string'
    
    // function processObject({ name, age }: { name: string; age: string }): void {
    //     console.log(name, age);
    // }
    // processObject({ name: "Alice", age: 30 }); // Error: Type 'number' is not assignable to type 'string'
    
    // function processMixed({ data: [first, second] }: { data: [string, string] }): void {
    //     console.log(first, second);
    // }
    // processMixed({ data: [1, 2] }); // Error: Type 'number' is not assignable to type 'string'
}

// Test with generic type errors
function testGenericTypeErrors() {
    // These should cause type errors
    // function processItems<T>(items: T[]): T[] {
    //     const [first, ...rest] = items;
    //     return [first, ...rest];
    // }
    // const result: string[] = processItems([1, 2, 3]); // Error: Type 'number[]' is not assignable to type 'string[]'
    
    // function processObject<T>(obj: T): T {
    //     const { ...rest } = obj;
    //     return rest;
    // }
    // const result: string = processObject({ name: "Alice" }); // Error: Type '{ name: string; }' is not assignable to type 'string'
}

// Test with async/await errors
function testAsyncErrors() {
    // These should cause type errors
    // async function processAsyncData(data: Promise<number[]>): Promise<number[]> {
    //     const [first, ...rest] = await data;
    //     return [first, ...rest];
    // }
    // const result: Promise<string[]> = processAsyncData(Promise.resolve([1, 2, 3])); // Error: Type 'Promise<number[]>' is not assignable to type 'Promise<string[]>'
    
    // async function processAsyncObject(data: Promise<{ name: string; age: number }>): Promise<{ name: string; age: number }> {
    //     const { name, age } = await data;
    //     return { name, age };
    // }
    // const result: Promise<{ name: string; age: string }> = processAsyncObject(Promise.resolve({ name: "Alice", age: 30 })); // Error: Type 'Promise<{ name: string; age: number }>' is not assignable to type 'Promise<{ name: string; age: string }>'
}

// Run tests
testErrorReporting();
testFunctionParameterErrors();
testGenericTypeErrors();
testAsyncErrors();
