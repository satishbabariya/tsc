#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

// Function System Testing Suite
class FunctionTestingSuite {
public:
    void runComprehensiveTesting() {
        std::cout << "=== Function System Testing Suite ===" << std::endl;
        
        // Test 1: Function Overloading
        testFunctionOverloading();
        
        // Test 2: Default Parameters
        testDefaultParameters();
        
        // Test 3: Rest Parameters
        testRestParameters();
        
        // Test 4: Arrow Functions
        testArrowFunctions();
        
        // Test 5: Complex Function Scenarios
        testComplexFunctionScenarios();
        
        // Test 6: Error Scenarios
        testErrorScenarios();
        
        std::cout << "=== Function System Testing Complete ===" << std::endl;
    }
    
private:
    void testFunctionOverloading() {
        std::cout << "\n--- Testing Function Overloading ---" << std::endl;
        
        std::string overloadTest = R"(
// Function overloading testing
function processValue(value: string): string;
function processValue(value: number): number;
function processValue(value: boolean): boolean;
function processValue(value: any): any {
    if (typeof value === "string") {
        return value.toUpperCase();
    } else if (typeof value === "number") {
        return value * 2;
    } else if (typeof value === "boolean") {
        return !value;
    }
    return value;
}

// Test function overloading
console.log("String:", processValue("hello"));
console.log("Number:", processValue(42));
console.log("Boolean:", processValue(true));

// Function overloading with different parameter counts
function createUser(name: string): User;
function createUser(name: string, age: number): User;
function createUser(name: string, age: number, email: string): User;
function createUser(name: string, age?: number, email?: string): User {
    return {
        name,
        age: age || 0,
        email: email || "no-email"
    };
}

interface User {
    name: string;
    age: number;
    email: string;
}

// Test different parameter counts
const user1 = createUser("Alice");
const user2 = createUser("Bob", 30);
const user3 = createUser("Charlie", 25, "charlie@example.com");

console.log("User 1:", user1);
console.log("User 2:", user2);
console.log("User 3:", user3);

// Function overloading with union types
function formatValue(value: string | number): string;
function formatValue(value: boolean): string;
function formatValue(value: any): string {
    if (typeof value === "string") {
        return `"${value}"`;
    } else if (typeof value === "number") {
        return value.toString();
    } else if (typeof value === "boolean") {
        return value ? "true" : "false";
    }
    return String(value);
}

console.log("Formatted string:", formatValue("hello"));
console.log("Formatted number:", formatValue(42));
console.log("Formatted boolean:", formatValue(true));
)";
        
        saveTestFile("function_overloading.ts", overloadTest);
        std::cout << "Generated function overloading test" << std::endl;
    }
    
    void testDefaultParameters() {
        std::cout << "\n--- Testing Default Parameters ---" << std::endl;
        
        std::string defaultTest = R"(
// Default parameters testing
function greet(name: string, greeting: string = "Hello"): string {
    return `${greeting}, ${name}!`;
}

// Test default parameters
console.log("Default greeting:", greet("Alice"));
console.log("Custom greeting:", greet("Bob", "Hi"));

// Multiple default parameters
function createMessage(text: string, prefix: string = "[INFO]", suffix: string = ""): string {
    return `${prefix} ${text}${suffix}`;
}

console.log("Default message:", createMessage("Hello world"));
console.log("Custom prefix:", createMessage("Hello world", "[WARNING]"));
console.log("Custom suffix:", createMessage("Hello world", "[INFO]", "!"));

// Default parameters with expressions
function calculateArea(width: number, height: number = width): number {
    return width * height;
}

console.log("Square area:", calculateArea(5));
console.log("Rectangle area:", calculateArea(5, 10));

// Default parameters with function calls
function getCurrentTime(): string {
    return new Date().toISOString();
}

function logMessage(message: string, timestamp: string = getCurrentTime()): void {
    console.log(`[${timestamp}] ${message}`);
}

logMessage("Application started");
logMessage("User logged in", "2024-01-01T00:00:00Z");

// Default parameters with destructuring
function processConfig(config: { name: string; port: number = 3000; debug: boolean = false }): void {
    console.log("Config:", config);
}

processConfig({ name: "myapp" });
processConfig({ name: "myapp", port: 8080 });
processConfig({ name: "myapp", port: 8080, debug: true });
)";
        
        saveTestFile("default_parameters.ts", defaultTest);
        std::cout << "Generated default parameters test" << std::endl;
    }
    
    void testRestParameters() {
        std::cout << "\n--- Testing Rest Parameters ---" << std::endl;
        
        std::string restTest = R"(
// Rest parameters testing
function sum(...numbers: number[]): number {
    return numbers.reduce((total, num) => total + num, 0);
}

// Test rest parameters
console.log("Sum of 1, 2, 3:", sum(1, 2, 3));
console.log("Sum of 1, 2, 3, 4, 5:", sum(1, 2, 3, 4, 5));
console.log("Sum of no numbers:", sum());

// Rest parameters with other parameters
function formatMessage(prefix: string, ...messages: string[]): string {
    return `${prefix}: ${messages.join(" ")}`;
}

console.log("Formatted message:", formatMessage("INFO", "Hello", "world", "from", "TypeScript"));

// Rest parameters with different types
function processItems(...items: (string | number)[]): void {
    items.forEach((item, index) => {
        console.log(`Item ${index}:`, item, typeof item);
    });
}

processItems("hello", 42, "world", 100);

// Rest parameters in arrow functions
const multiply = (...numbers: number[]): number => {
    return numbers.reduce((total, num) => total * num, 1);
};

console.log("Multiply 2, 3, 4:", multiply(2, 3, 4));

// Rest parameters with destructuring
function processUserData(name: string, ...details: [string, any][]): void {
    console.log("Name:", name);
    details.forEach(([key, value]) => {
        console.log(`${key}:`, value);
    });
}

processUserData("Alice", ["age", 30], ["city", "New York"], ["country", "USA"]);

// Rest parameters with type constraints
function processNumbers<T extends number>(...numbers: T[]): T[] {
    return numbers.map(num => num * 2 as T);
}

const doubled = processNumbers(1, 2, 3, 4, 5);
console.log("Doubled numbers:", doubled);
)";
        
        saveTestFile("rest_parameters.ts", restTest);
        std::cout << "Generated rest parameters test" << std::endl;
    }
    
    void testArrowFunctions() {
        std::cout << "\n--- Testing Arrow Functions ---" << std::endl;
        
        std::string arrowTest = R"(
// Arrow functions testing
// Basic arrow function
const add = (a: number, b: number): number => a + b;
console.log("Add:", add(5, 3));

// Arrow function with block body
const multiply = (a: number, b: number): number => {
    const result = a * b;
    return result;
};
console.log("Multiply:", multiply(4, 6));

// Single parameter arrow function
const square = (x: number): number => x * x;
console.log("Square:", square(5));

// Arrow function with no parameters
const getCurrentTime = (): string => new Date().toISOString();
console.log("Current time:", getCurrentTime());

// Arrow function with destructuring
const processUser = ({ name, age }: { name: string; age: number }): string => {
    return `${name} is ${age} years old`;
};

const user = { name: "Alice", age: 30 };
console.log("User info:", processUser(user));

// Arrow function with rest parameters
const concatenate = (...strings: string[]): string => strings.join("");
console.log("Concatenated:", concatenate("Hello", " ", "World"));

// Arrow function in array methods
const numbers = [1, 2, 3, 4, 5];
const doubled = numbers.map(n => n * 2);
const filtered = numbers.filter(n => n > 2);
const sum = numbers.reduce((total, n) => total + n, 0);

console.log("Original:", numbers);
console.log("Doubled:", doubled);
console.log("Filtered:", filtered);
console.log("Sum:", sum);

// Arrow function with this binding
class Calculator {
    private value: number = 0;
    
    add = (num: number): Calculator => {
        this.value += num;
        return this;
    };
    
    multiply = (num: number): Calculator => {
        this.value *= num;
        return this;
    };
    
    getValue = (): number => this.value;
}

const calc = new Calculator();
const result = calc.add(5).multiply(2).add(3).getValue();
console.log("Calculator result:", result);

// Arrow function with async/await
const fetchData = async (url: string): Promise<any> => {
    // Simulate async operation
    return new Promise(resolve => {
        setTimeout(() => resolve({ data: "Hello from " + url }), 100);
    });
};

// Arrow function with generics
const identity = <T>(value: T): T => value;
const processArray = <T>(array: T[], processor: (item: T) => T): T[] => {
    return array.map(processor);
};

const processedNumbers = processArray([1, 2, 3], n => n * 2);
console.log("Processed numbers:", processedNumbers);
)";
        
        saveTestFile("arrow_functions.ts", arrowTest);
        std::cout << "Generated arrow functions test" << std::endl;
    }
    
    void testComplexFunctionScenarios() {
        std::cout << "\n--- Testing Complex Function Scenarios ---" << std::endl;
        
        std::string complexTest = R"(
// Complex function scenarios testing
interface ApiResponse<T> {
    data: T;
    status: number;
    message: string;
}

// Generic function with overloads
function apiCall<T>(url: string): Promise<ApiResponse<T>>;
function apiCall<T>(url: string, options: { method: string; body?: any }): Promise<ApiResponse<T>>;
function apiCall<T>(url: string, options?: { method: string; body?: any }): Promise<ApiResponse<T>> {
    return new Promise(resolve => {
        setTimeout(() => {
            resolve({
                data: {} as T,
                status: 200,
                message: "Success"
            });
        }, 100);
    });
}

// Test generic function with overloads
apiCall<User>("/api/users").then(response => {
    console.log("API response:", response);
});

// Function with complex parameter types
function processData<T extends Record<string, any>>(
    data: T,
    transformer: (item: T) => T,
    validator?: (item: T) => boolean
): T {
    const transformed = transformer(data);
    if (validator && !validator(transformed)) {
        throw new Error("Validation failed");
    }
    return transformed;
}

interface User {
    id: number;
    name: string;
    email: string;
}

const userData: User = { id: 1, name: "Alice", email: "alice@example.com" };

const processedUser = processData(
    userData,
    (user) => ({ ...user, name: user.name.toUpperCase() }),
    (user) => user.email.includes("@")
);

console.log("Processed user:", processedUser);

// Function with callback and error handling
function withErrorHandling<T>(
    operation: () => T,
    onError: (error: Error) => void
): T | null {
    try {
        return operation();
    } catch (error) {
        onError(error as Error);
        return null;
    }
}

const result = withErrorHandling(
    () => {
        // Simulate operation that might fail
        if (Math.random() > 0.5) {
            throw new Error("Random failure");
        }
        return "Success!";
    },
    (error) => console.error("Error occurred:", error.message)
);

console.log("Operation result:", result);

// Function with conditional return types
function getValue<T>(value: T): T extends string ? string : T extends number ? number : T {
    return value as any;
}

const stringValue = getValue("hello");
const numberValue = getValue(42);
const booleanValue = getValue(true);

console.log("String value:", stringValue);
console.log("Number value:", numberValue);
console.log("Boolean value:", booleanValue);

// Function with method chaining
class QueryBuilder<T> {
    private conditions: Array<(item: T) => boolean> = [];
    
    where = (condition: (item: T) => boolean): QueryBuilder<T> => {
        this.conditions.push(condition);
        return this;
    };
    
    execute = (data: T[]): T[] => {
        return data.filter(item => 
            this.conditions.every(condition => condition(item))
        );
    };
}

const users: User[] = [
    { id: 1, name: "Alice", email: "alice@example.com" },
    { id: 2, name: "Bob", email: "bob@example.com" },
    { id: 3, name: "Charlie", email: "charlie@example.com" }
];

const query = new QueryBuilder<User>();
const filteredUsers = query
    .where(user => user.id > 1)
    .where(user => user.name.startsWith("B"))
    .execute(users);

console.log("Filtered users:", filteredUsers);
)";
        
        saveTestFile("complex_function_scenarios.ts", complexTest);
        std::cout << "Generated complex function scenarios test" << std::endl;
    }
    
    void testErrorScenarios() {
        std::cout << "\n--- Testing Error Scenarios ---" << std::endl;
        
        std::string errorTest = R"(
// Error scenarios testing
// Function with invalid parameter types
function processValue(value: string): string {
    return value.toUpperCase();
}

// These should cause type errors
// processValue(42); // Error: Argument of type 'number' is not assignable to parameter of type 'string'
// processValue(true); // Error: Argument of type 'boolean' is not assignable to parameter of type 'string'

// Function with missing required parameters
function createUser(name: string, age: number, email: string): User {
    return { name, age, email };
}

// These should cause errors
// createUser("Alice"); // Error: Expected 3 arguments, but got 1
// createUser("Alice", 30); // Error: Expected 3 arguments, but got 2

interface User {
    name: string;
    age: number;
    email: string;
}

// Function with invalid return type
function getStringValue(): string {
    return 42; // Error: Type 'number' is not assignable to type 'string'
}

// Function with invalid default parameter type
function processData(data: string, count: number = "invalid"): void {
    console.log(data, count);
}

// Function with invalid rest parameter type
function processItems(...items: string[]): void {
    items.forEach(item => console.log(item));
}

// These should cause errors
// processItems("hello", 42, "world"); // Error: Argument of type 'number' is not assignable to parameter of type 'string'

// Function with conflicting overloads
function conflict(value: string): string;
function conflict(value: string): number; // Error: Duplicate function implementation
function conflict(value: string): string | number {
    return value;
}

// Function with invalid arrow function syntax
// const invalidArrow = (a, b) => { return a + b; }; // Error: Parameter 'a' implicitly has an 'any' type

// Function with invalid async usage
async function asyncFunction(): Promise<string> {
    return "Hello";
}

// This should cause an error
// const result = asyncFunction(); // Error: 'await' expression is only allowed within an async function

// Function with invalid generic constraints
function processGeneric<T extends string>(value: T): T {
    return value;
}

// This should cause an error
// processGeneric(42); // Error: Argument of type 'number' is not assignable to parameter of type 'string'

// Function with invalid destructuring
function processObject({ name, age }: { name: string; age: number }): void {
    console.log(name, age);
}

// This should cause an error
// processObject({ name: "Alice" }); // Error: Property 'age' is missing

// Function with invalid method chaining
class InvalidBuilder {
    method1(): InvalidBuilder {
        return this;
    }
    
    method2(): string { // Error: Method 'method2' of type 'InvalidBuilder' is not assignable to the same method in base type 'InvalidBuilder'
        return "invalid";
    }
}

// Function with invalid callback types
function withCallback(callback: (value: string) => void): void {
    callback("hello");
}

// This should cause an error
// withCallback((value: number) => console.log(value)); // Error: Argument of type '(value: number) => void' is not assignable to parameter of type '(value: string) => void'
)";
        
        saveTestFile("error_scenarios.ts", errorTest);
        std::cout << "Generated error scenarios test" << std::endl;
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
        FunctionTestingSuite suite;
        suite.runComprehensiveTesting();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}