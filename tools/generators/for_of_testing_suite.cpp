#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

// For...of Loop Testing Suite
class ForOfTestingSuite {
public:
    void runComprehensiveTesting() {
        std::cout << "=== For...of Loop Testing Suite ===" << std::endl;
        
        // Test 1: Basic for...of loops
        testBasicForOfLoops();
        
        // Test 2: Array iteration
        testArrayIteration();
        
        // Test 3: String iteration
        testStringIteration();
        
        // Test 4: Object iteration
        testObjectIteration();
        
        // Test 5: Custom iterables
        testCustomIterables();
        
        // Test 6: Async for...of loops
        testAsyncForOfLoops();
        
        // Test 7: Type checking scenarios
        testTypeCheckingScenarios();
        
        // Test 8: Error scenarios
        testErrorScenarios();
        
        // Test 9: Performance scenarios
        testPerformanceScenarios();
        
        std::cout << "=== For...of Loop Testing Complete ===" << std::endl;
    }
    
private:
    void testBasicForOfLoops() {
        std::cout << "\n--- Testing Basic For...of Loops ---" << std::endl;
        
        // Test: Basic array iteration
        std::string basicTest = R"(
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
)";
        
        saveTestFile("basic_for_of.ts", basicTest);
        
        std::cout << "Generated basic for...of test" << std::endl;
    }
    
    void testArrayIteration() {
        std::cout << "\n--- Testing Array Iteration ---" << std::endl;
        
        // Test: Advanced array iteration
        std::string arrayTest = R"(
// Advanced array iteration testing
interface Person {
    name: string;
    age: number;
    city: string;
}

function testArrayIteration() {
    const people: Person[] = [
        { name: "Alice", age: 30, city: "New York" },
        { name: "Bob", age: 25, city: "London" },
        { name: "Charlie", age: 35, city: "Tokyo" },
        { name: "Diana", age: 28, city: "Paris" }
    ];
    
    // Iterate over people
    const names: string[] = [];
    for (const person of people) {
        names.push(person.name);
    }
    
    console.log("Names:", names);
    
    // Filter and iterate
    const youngPeople: Person[] = [];
    for (const person of people) {
        if (person.age < 30) {
            youngPeople.push(person);
        }
    }
    
    console.log("Young people:", youngPeople);
    
    // Transform and iterate
    const ages: number[] = [];
    for (const person of people) {
        ages.push(person.age);
    }
    
    console.log("Ages:", ages);
}

// Test with nested arrays
function testNestedArrays() {
    const matrix = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ];
    
    const flattened: number[] = [];
    for (const row of matrix) {
        for (const cell of row) {
            flattened.push(cell);
        }
    }
    
    console.log("Flattened matrix:", flattened);
    
    // Sum each row
    const rowSums: number[] = [];
    for (const row of matrix) {
        let sum = 0;
        for (const cell of row) {
            sum += cell;
        }
        rowSums.push(sum);
    }
    
    console.log("Row sums:", rowSums);
}

// Test with array methods and for...of
function testArrayMethodsWithForOf() {
    const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    
    // Filter even numbers and iterate
    const evenNumbers = numbers.filter(n => n % 2 === 0);
    const doubledEvens: number[] = [];
    
    for (const num of evenNumbers) {
        doubledEvens.push(num * 2);
    }
    
    console.log("Doubled evens:", doubledEvens);
    
    // Map and iterate
    const squares = numbers.map(n => n * n);
    const squareRoots: number[] = [];
    
    for (const square of squares) {
        squareRoots.push(Math.sqrt(square));
    }
    
    console.log("Square roots:", squareRoots);
}

// Run tests
testArrayIteration();
testNestedArrays();
testArrayMethodsWithForOf();
)";
        
        saveTestFile("array_iteration.ts", arrayTest);
        
        std::cout << "Generated array iteration test" << std::endl;
    }
    
    void testStringIteration() {
        std::cout << "\n--- Testing String Iteration ---" << std::endl;
        
        // Test: String iteration
        std::string stringTest = R"(
// String iteration testing
function testStringIteration() {
    const text = "Hello, TypeScript!";
    const characters: string[] = [];
    
    // Iterate over string characters
    for (const char of text) {
        characters.push(char);
    }
    
    console.log("Characters:", characters);
    
    // Count vowels
    const vowels = "aeiouAEIOU";
    let vowelCount = 0;
    
    for (const char of text) {
        if (vowels.includes(char)) {
            vowelCount++;
        }
    }
    
    console.log("Vowel count:", vowelCount);
    
    // Count consonants
    let consonantCount = 0;
    for (const char of text) {
        if (/[a-zA-Z]/.test(char) && !vowels.includes(char)) {
            consonantCount++;
        }
    }
    
    console.log("Consonant count:", consonantCount);
}

// Test with multiple strings
function testMultipleStrings() {
    const words = ["hello", "world", "typescript", "programming"];
    const wordLengths: number[] = [];
    
    for (const word of words) {
        wordLengths.push(word.length);
    }
    
    console.log("Word lengths:", wordLengths);
    
    // Find longest word
    let longestWord = "";
    let maxLength = 0;
    
    for (const word of words) {
        if (word.length > maxLength) {
            maxLength = word.length;
            longestWord = word;
        }
    }
    
    console.log("Longest word:", longestWord, "Length:", maxLength);
}

// Test with string manipulation
function testStringManipulation() {
    const sentences = [
        "Hello world!",
        "TypeScript is awesome!",
        "Programming is fun!",
        "Code quality matters!"
    ];
    
    const wordCounts: number[] = [];
    
    for (const sentence of sentences) {
        const words = sentence.split(" ");
        wordCounts.push(words.length);
    }
    
    console.log("Word counts:", wordCounts);
    
    // Count total words
    let totalWords = 0;
    for (const sentence of sentences) {
        const words = sentence.split(" ");
        totalWords += words.length;
    }
    
    console.log("Total words:", totalWords);
}

// Run tests
testStringIteration();
testMultipleStrings();
testStringManipulation();
)";
        
        saveTestFile("string_iteration.ts", stringTest);
        
        std::cout << "Generated string iteration test" << std::endl;
    }
    
    void testObjectIteration() {
        std::cout << "\n--- Testing Object Iteration ---" << std::endl;
        
        // Test: Object iteration with for...of
        std::string objectTest = R"(
// Object iteration testing
interface User {
    id: number;
    name: string;
    email: string;
    isActive: boolean;
}

function testObjectIteration() {
    const users: User[] = [
        { id: 1, name: "Alice", email: "alice@example.com", isActive: true },
        { id: 2, name: "Bob", email: "bob@example.com", isActive: false },
        { id: 3, name: "Charlie", email: "charlie@example.com", isActive: true },
        { id: 4, name: "Diana", email: "diana@example.com", isActive: true }
    ];
    
    // Iterate over users
    const activeUsers: User[] = [];
    for (const user of users) {
        if (user.isActive) {
            activeUsers.push(user);
        }
    }
    
    console.log("Active users:", activeUsers);
    
    // Extract emails
    const emails: string[] = [];
    for (const user of users) {
        emails.push(user.email);
    }
    
    console.log("Emails:", emails);
    
    // Count active users
    let activeCount = 0;
    for (const user of users) {
        if (user.isActive) {
            activeCount++;
        }
    }
    
    console.log("Active user count:", activeCount);
}

// Test with object properties
function testObjectProperties() {
    const config = {
        apiUrl: "https://api.example.com",
        timeout: 5000,
        retries: 3,
        debug: true
    };
    
    const configValues: any[] = [];
    for (const value of Object.values(config)) {
        configValues.push(value);
    }
    
    console.log("Config values:", configValues);
    
    const configKeys: string[] = [];
    for (const key of Object.keys(config)) {
        configKeys.push(key);
    }
    
    console.log("Config keys:", configKeys);
    
    const configEntries: [string, any][] = [];
    for (const entry of Object.entries(config)) {
        configEntries.push(entry);
    }
    
    console.log("Config entries:", configEntries);
}

// Test with Map and Set
function testMapAndSet() {
    // Map iteration
    const userMap = new Map<number, string>();
    userMap.set(1, "Alice");
    userMap.set(2, "Bob");
    userMap.set(3, "Charlie");
    
    const mapValues: string[] = [];
    for (const value of userMap.values()) {
        mapValues.push(value);
    }
    
    console.log("Map values:", mapValues);
    
    const mapKeys: number[] = [];
    for (const key of userMap.keys()) {
        mapKeys.push(key);
    }
    
    console.log("Map keys:", mapKeys);
    
    const mapEntries: [number, string][] = [];
    for (const entry of userMap.entries()) {
        mapEntries.push(entry);
    }
    
    console.log("Map entries:", mapEntries);
    
    // Set iteration
    const numberSet = new Set<number>([1, 2, 3, 4, 5, 1, 2, 3]);
    const uniqueNumbers: number[] = [];
    
    for (const num of numberSet) {
        uniqueNumbers.push(num);
    }
    
    console.log("Unique numbers:", uniqueNumbers);
}

// Run tests
testObjectIteration();
testObjectProperties();
testMapAndSet();
)";
        
        saveTestFile("object_iteration.ts", objectTest);
        
        std::cout << "Generated object iteration test" << std::endl;
    }
    
    void testCustomIterables() {
        std::cout << "\n--- Testing Custom Iterables ---" << std::endl;
        
        // Test: Custom iterable classes
        std::string customTest = R"(
// Custom iterable testing
class NumberRange implements Iterable<number> {
    private start: number;
    private end: number;
    private step: number;
    
    constructor(start: number, end: number, step: number = 1) {
        this.start = start;
        this.end = end;
        this.step = step;
    }
    
    *[Symbol.iterator](): Iterator<number> {
        for (let i = this.start; i < this.end; i += this.step) {
            yield i;
        }
    }
}

class FibonacciSequence implements Iterable<number> {
    private count: number;
    
    constructor(count: number) {
        this.count = count;
    }
    
    *[Symbol.iterator](): Iterator<number> {
        let a = 0, b = 1;
        for (let i = 0; i < this.count; i++) {
            yield a;
            [a, b] = [b, a + b];
        }
    }
}

class StringWords implements Iterable<string> {
    private text: string;
    
    constructor(text: string) {
        this.text = text;
    }
    
    *[Symbol.iterator](): Iterator<string> {
        const words = this.text.split(/\s+/);
        for (const word of words) {
            yield word;
        }
    }
}

// Test custom iterables
function testCustomIterables() {
    // Test NumberRange
    const range = new NumberRange(1, 10, 2);
    const oddNumbers: number[] = [];
    
    for (const num of range) {
        oddNumbers.push(num);
    }
    
    console.log("Odd numbers:", oddNumbers);
    
    // Test FibonacciSequence
    const fibonacci = new FibonacciSequence(10);
    const fibNumbers: number[] = [];
    
    for (const num of fibonacci) {
        fibNumbers.push(num);
    }
    
    console.log("Fibonacci numbers:", fibNumbers);
    
    // Test StringWords
    const words = new StringWords("Hello world TypeScript programming");
    const wordList: string[] = [];
    
    for (const word of words) {
        wordList.push(word);
    }
    
    console.log("Words:", wordList);
}

// Test with async iterable
class AsyncNumberGenerator implements AsyncIterable<number> {
    private count: number;
    private delay: number;
    
    constructor(count: number, delay: number = 100) {
        this.count = count;
        this.delay = delay;
    }
    
    async *[Symbol.asyncIterator](): AsyncIterator<number> {
        for (let i = 0; i < this.count; i++) {
            await new Promise(resolve => setTimeout(resolve, this.delay));
            yield i;
        }
    }
}

// Test async iterable
async function testAsyncIterable() {
    const asyncGen = new AsyncNumberGenerator(5, 50);
    const asyncNumbers: number[] = [];
    
    for await (const num of asyncGen) {
        asyncNumbers.push(num);
    }
    
    console.log("Async numbers:", asyncNumbers);
}

// Test with generator functions
function* numberGenerator(start: number, end: number): Generator<number> {
    for (let i = start; i < end; i++) {
        yield i;
    }
}

function* fibonacciGenerator(count: number): Generator<number> {
    let a = 0, b = 1;
    for (let i = 0; i < count; i++) {
        yield a;
        [a, b] = [b, a + b];
    }
}

function testGeneratorFunctions() {
    // Test number generator
    const numbers: number[] = [];
    for (const num of numberGenerator(1, 6)) {
        numbers.push(num);
    }
    
    console.log("Generated numbers:", numbers);
    
    // Test fibonacci generator
    const fibNumbers: number[] = [];
    for (const num of fibonacciGenerator(8)) {
        fibNumbers.push(num);
    }
    
    console.log("Generated fibonacci:", fibNumbers);
}

// Run tests
testCustomIterables();
testGeneratorFunctions();
// testAsyncIterable(); // Uncomment for async testing
)";
        
        saveTestFile("custom_iterables.ts", customTest);
        
        std::cout << "Generated custom iterables test" << std::endl;
    }
    
    void testAsyncForOfLoops() {
        std::cout << "\n--- Testing Async For...of Loops ---" << std::endl;
        
        // Test: Async for...of loops
        std::string asyncTest = R"(
// Async for...of loop testing
class AsyncDataProcessor {
    private data: number[];
    
    constructor(data: number[]) {
        this.data = data;
    }
    
    async *processAsync(): AsyncGenerator<number> {
        for (const item of this.data) {
            // Simulate async processing
            await new Promise(resolve => setTimeout(resolve, 100));
            yield item * 2;
        }
    }
}

class AsyncFileReader {
    private files: string[];
    
    constructor(files: string[]) {
        this.files = files;
    }
    
    async *readFiles(): AsyncGenerator<string> {
        for (const file of this.files) {
            // Simulate file reading
            await new Promise(resolve => setTimeout(resolve, 200));
            yield `Content of ${file}`;
        }
    }
}

// Test async for...of loops
async function testAsyncForOf() {
    const processor = new AsyncDataProcessor([1, 2, 3, 4, 5]);
    const processedData: number[] = [];
    
    // Async for...of loop
    for await (const item of processor.processAsync()) {
        processedData.push(item);
    }
    
    console.log("Processed data:", processedData);
}

async function testAsyncFileReading() {
    const reader = new AsyncFileReader(["file1.txt", "file2.txt", "file3.txt"]);
    const fileContents: string[] = [];
    
    // Async for...of loop
    for await (const content of reader.readFiles()) {
        fileContents.push(content);
    }
    
    console.log("File contents:", fileContents);
}

// Test with Promise.all and async iteration
async function testPromiseAllWithAsyncIteration() {
    const urls = [
        "https://api.example.com/users",
        "https://api.example.com/posts",
        "https://api.example.com/comments"
    ];
    
    const results: string[] = [];
    
    // Simulate async API calls
    for (const url of urls) {
        const result = await fetchData(url);
        results.push(result);
    }
    
    console.log("API results:", results);
}

// Mock fetch function
async function fetchData(url: string): Promise<string> {
    // Simulate API call
    await new Promise(resolve => setTimeout(resolve, 300));
    return `Data from ${url}`;
}

// Test with async generator
async function* asyncNumberGenerator(count: number): AsyncGenerator<number> {
    for (let i = 0; i < count; i++) {
        await new Promise(resolve => setTimeout(resolve, 100));
        yield i;
    }
}

async function testAsyncGenerator() {
    const numbers: number[] = [];
    
    for await (const num of asyncNumberGenerator(5)) {
        numbers.push(num);
    }
    
    console.log("Async generated numbers:", numbers);
}

// Run async tests
async function runAsyncTests() {
    await testAsyncForOf();
    await testAsyncFileReading();
    await testPromiseAllWithAsyncIteration();
    await testAsyncGenerator();
}

// runAsyncTests(); // Uncomment for async testing
)";
        
        saveTestFile("async_for_of.ts", asyncTest);
        
        std::cout << "Generated async for...of test" << std::endl;
    }
    
    void testTypeCheckingScenarios() {
        std::cout << "\n--- Testing Type Checking Scenarios ---" << std::endl;
        
        // Test: Type checking scenarios
        std::string typeTest = R"(
// Type checking scenarios for for...of loops
interface Product {
    id: number;
    name: string;
    price: number;
    category: string;
}

interface Order {
    id: number;
    userId: number;
    products: Product[];
    total: number;
}

function testTypeChecking() {
    const products: Product[] = [
        { id: 1, name: "Laptop", price: 999.99, category: "Electronics" },
        { id: 2, name: "Mouse", price: 29.99, category: "Electronics" },
        { id: 3, name: "Book", price: 19.99, category: "Books" }
    ];
    
    // Type-safe iteration
    const productNames: string[] = [];
    for (const product of products) {
        productNames.push(product.name);
    }
    
    console.log("Product names:", productNames);
    
    // Type-safe filtering
    const expensiveProducts: Product[] = [];
    for (const product of products) {
        if (product.price > 50) {
            expensiveProducts.push(product);
        }
    }
    
    console.log("Expensive products:", expensiveProducts);
    
    // Type-safe transformation
    const productPrices: number[] = [];
    for (const product of products) {
        productPrices.push(product.price);
    }
    
    console.log("Product prices:", productPrices);
}

// Test with generic types
function testGenericTypes<T>(items: T[]): T[] {
    const results: T[] = [];
    
    for (const item of items) {
        results.push(item);
    }
    
    return results;
}

// Test with union types
function testUnionTypes() {
    const mixedArray: (string | number | boolean)[] = [
        "hello", 42, true, "world", 100, false
    ];
    
    const strings: string[] = [];
    const numbers: number[] = [];
    const booleans: boolean[] = [];
    
    for (const item of mixedArray) {
        if (typeof item === "string") {
            strings.push(item);
        } else if (typeof item === "number") {
            numbers.push(item);
        } else if (typeof item === "boolean") {
            booleans.push(item);
        }
    }
    
    console.log("Strings:", strings);
    console.log("Numbers:", numbers);
    console.log("Booleans:", booleans);
}

// Test with optional properties
interface User {
    id: number;
    name: string;
    email?: string;
    age?: number;
}

function testOptionalProperties() {
    const users: User[] = [
        { id: 1, name: "Alice", email: "alice@example.com", age: 30 },
        { id: 2, name: "Bob", age: 25 },
        { id: 3, name: "Charlie", email: "charlie@example.com" }
    ];
    
    const emails: string[] = [];
    const ages: number[] = [];
    
    for (const user of users) {
        if (user.email) {
            emails.push(user.email);
        }
        if (user.age) {
            ages.push(user.age);
        }
    }
    
    console.log("Emails:", emails);
    console.log("Ages:", ages);
}

// Test with nested objects
interface Company {
    name: string;
    employees: User[];
    departments: string[];
}

function testNestedObjects() {
    const companies: Company[] = [
        {
            name: "Tech Corp",
            employees: [
                { id: 1, name: "Alice", email: "alice@techcorp.com", age: 30 },
                { id: 2, name: "Bob", email: "bob@techcorp.com", age: 25 }
            ],
            departments: ["Engineering", "Marketing", "Sales"]
        },
        {
            name: "Startup Inc",
            employees: [
                { id: 3, name: "Charlie", email: "charlie@startup.com", age: 28 }
            ],
            departments: ["Engineering", "Design"]
        }
    ];
    
    const allEmployees: User[] = [];
    const allDepartments: string[] = [];
    
    for (const company of companies) {
        for (const employee of company.employees) {
            allEmployees.push(employee);
        }
        for (const department of company.departments) {
            allDepartments.push(department);
        }
    }
    
    console.log("All employees:", allEmployees);
    console.log("All departments:", allDepartments);
}

// Run tests
testTypeChecking();
testUnionTypes();
testOptionalProperties();
testNestedObjects();

// Test generic function
const stringArray = testGenericTypes(["hello", "world", "typescript"]);
const numberArray = testGenericTypes([1, 2, 3, 4, 5]);
const booleanArray = testGenericTypes([true, false, true]);

console.log("Generic string array:", stringArray);
console.log("Generic number array:", numberArray);
console.log("Generic boolean array:", booleanArray);
)";
        
        saveTestFile("type_checking_scenarios.ts", typeTest);
        
        std::cout << "Generated type checking scenarios test" << std::endl;
    }
    
    void testErrorScenarios() {
        std::cout << "\n--- Testing Error Scenarios ---" << std::endl;
        
        // Test: Error scenarios
        std::string errorTest = R"(
// Error scenarios for for...of loops
function testErrorScenarios() {
    // Test with null/undefined
    const nullArray: number[] | null = null;
    const undefinedArray: number[] | undefined = undefined;
    
    // These should cause errors
    // for (const item of nullArray) { // Error: Object is possibly 'null'
    //     console.log(item);
    // }
    
    // for (const item of undefinedArray) { // Error: Object is possibly 'undefined'
    //     console.log(item);
    // }
    
    // Safe iteration with null checks
    if (nullArray) {
        for (const item of nullArray) {
            console.log("Null array item:", item);
        }
    }
    
    if (undefinedArray) {
        for (const item of undefinedArray) {
            console.log("Undefined array item:", item);
        }
    }
}

// Test with type mismatches
function testTypeMismatches() {
    const numbers: number[] = [1, 2, 3, 4, 5];
    
    // This should cause a type error
    // const strings: string[] = [];
    // for (const num of numbers) {
    //     strings.push(num); // Error: Type 'number' is not assignable to type 'string'
    // }
    
    // Correct way
    const strings: string[] = [];
    for (const num of numbers) {
        strings.push(num.toString());
    }
    
    console.log("Number strings:", strings);
}

// Test with non-iterable types
function testNonIterableTypes() {
    const notIterable = 42;
    const alsoNotIterable = { name: "test" };
    
    // These should cause errors
    // for (const item of notIterable) { // Error: Type 'number' is not iterable
    //     console.log(item);
    // }
    
    // for (const item of alsoNotIterable) { // Error: Type '{ name: string; }' is not iterable
    //     console.log(item);
    // }
}

// Test with async context errors
function testAsyncContextErrors() {
    const regularArray = [1, 2, 3, 4, 5];
    
    // This should cause an error in async context
    // async function asyncFunction() {
    //     for await (const item of regularArray) { // Error: Type 'number[]' is not async iterable
    //         console.log(item);
    //     }
    // }
}

// Test with variable redeclaration
function testVariableRedeclaration() {
    const numbers = [1, 2, 3, 4, 5];
    
    // This should cause an error
    // for (const numbers of numbers) { // Error: Cannot redeclare block-scoped variable 'numbers'
    //     console.log(numbers);
    // }
    
    // Correct way
    for (const num of numbers) {
        console.log("Number:", num);
    }
}

// Test with const reassignment
function testConstReassignment() {
    const numbers = [1, 2, 3, 4, 5];
    
    // This should cause an error
    // for (const num of numbers) {
    //     num = 10; // Error: Cannot assign to 'num' because it is a constant
    // }
    
    // Use let if reassignment is needed
    for (let num of numbers) {
        num = num * 2;
        console.log("Doubled:", num);
    }
}

// Run tests
testErrorScenarios();
testTypeMismatches();
testNonIterableTypes();
testAsyncContextErrors();
testVariableRedeclaration();
testConstReassignment();
)";
        
        saveTestFile("error_scenarios.ts", errorTest);
        
        std::cout << "Generated error scenarios test" << std::endl;
    }
    
    void testPerformanceScenarios() {
        std::cout << "\n--- Testing Performance Scenarios ---" << std::endl;
        
        // Test: Performance scenarios
        std::string performanceTest = R"(
// Performance scenarios for for...of loops
function testPerformanceScenarios() {
    // Test with large arrays
    const largeArray = Array.from({ length: 100000 }, (_, i) => i);
    
    console.time("for...of loop");
    let sum = 0;
    for (const num of largeArray) {
        sum += num;
    }
    console.timeEnd("for...of loop");
    console.log("Sum:", sum);
    
    // Test with nested loops
    const matrix = Array.from({ length: 1000 }, () => 
        Array.from({ length: 1000 }, (_, i) => Math.random())
    );
    
    console.time("nested for...of loops");
    let total = 0;
    for (const row of matrix) {
        for (const cell of row) {
            total += cell;
        }
    }
    console.timeEnd("nested for...of loops");
    console.log("Total:", total);
    
    // Test with object iteration
    const largeObject = Object.fromEntries(
        Array.from({ length: 10000 }, (_, i) => [`key${i}`, i])
    );
    
    console.time("object iteration");
    const values: number[] = [];
    for (const value of Object.values(largeObject)) {
        values.push(value);
    }
    console.timeEnd("object iteration");
    console.log("Values count:", values.length);
}

// Test with different iteration methods
function testIterationMethods() {
    const numbers = Array.from({ length: 100000 }, (_, i) => i);
    
    // for...of loop
    console.time("for...of");
    let sum1 = 0;
    for (const num of numbers) {
        sum1 += num;
    }
    console.timeEnd("for...of");
    
    // traditional for loop
    console.time("for loop");
    let sum2 = 0;
    for (let i = 0; i < numbers.length; i++) {
        sum2 += numbers[i];
    }
    console.timeEnd("for loop");
    
    // forEach
    console.time("forEach");
    let sum3 = 0;
    numbers.forEach(num => {
        sum3 += num;
    });
    console.timeEnd("forEach");
    
    // map
    console.time("map");
    const doubled = numbers.map(num => num * 2);
    console.timeEnd("map");
    
    console.log("Sums:", sum1, sum2, sum3);
    console.log("Doubled length:", doubled.length);
}

// Test with memory usage
function testMemoryUsage() {
    const createLargeArray = (size: number) => 
        Array.from({ length: size }, (_, i) => ({ id: i, data: `data${i}` }));
    
    // Test with different array sizes
    const sizes = [1000, 10000, 100000];
    
    for (const size of sizes) {
        console.log(`Testing with array size: ${size}`);
        
        const array = createLargeArray(size);
        
        console.time(`iteration-${size}`);
        const results: string[] = [];
        for (const item of array) {
            results.push(item.data);
        }
        console.timeEnd(`iteration-${size}`);
        
        console.log(`Results count: ${results.length}`);
        
        // Clear array to free memory
        array.length = 0;
    }
}

// Test with async performance
async function testAsyncPerformance() {
    const createAsyncGenerator = (count: number, delay: number) => 
        async function* () {
            for (let i = 0; i < count; i++) {
                await new Promise(resolve => setTimeout(resolve, delay));
                yield i;
            }
        };
    
    const asyncGen = createAsyncGenerator(100, 10);
    
    console.time("async for...of");
    const asyncResults: number[] = [];
    for await (const num of asyncGen()) {
        asyncResults.push(num);
    }
    console.timeEnd("async for...of");
    
    console.log("Async results count:", asyncResults.length);
}

// Test with break and continue
function testBreakAndContinue() {
    const numbers = Array.from({ length: 1000000 }, (_, i) => i);
    
    console.time("for...of with break");
    let sum = 0;
    for (const num of numbers) {
        sum += num;
        if (num > 100000) {
            break;
        }
    }
    console.timeEnd("for...of with break");
    console.log("Sum with break:", sum);
    
    console.time("for...of with continue");
    sum = 0;
    for (const num of numbers) {
        if (num % 2 === 0) {
            continue;
        }
        sum += num;
        if (num > 100000) {
            break;
        }
    }
    console.timeEnd("for...of with continue");
    console.log("Sum with continue:", sum);
}

// Run performance tests
testPerformanceScenarios();
testIterationMethods();
testMemoryUsage();
testBreakAndContinue();
// testAsyncPerformance(); // Uncomment for async testing
)";
        
        saveTestFile("performance_scenarios.ts", performanceTest);
        
        std::cout << "Generated performance scenarios test" << std::endl;
    }
    
    void saveTestFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (file.is_open()) {
            file << content;
            file.close();
            std::cout << "Generated: " << filename << std::endl;
        }
    }
};

int main() {
    try {
        ForOfTestingSuite suite;
        suite.runComprehensiveTesting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}