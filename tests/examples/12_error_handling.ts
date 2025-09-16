// Error Handling
// Demonstrates try-catch-finally, throw statements, and error types

// Basic try-catch block
function divide(a: number, b: number): number {
    try {
        if (b === 0) {
            throw new Error("Division by zero is not allowed");
        }
        return a / b;
    } catch (error) {
        _print("Error caught:", error.message);
        return 0;
    }
}

// Try-catch with different error types
function processString(input: string): string {
    try {
        if (input.length === 0) {
            throw new Error("Input string cannot be empty");
        }
        if (input.length > 100) {
            throw new Error("Input string is too long");
        }
        return input.toUpperCase();
    } catch (error) {
        _print("String processing error:", error.message);
        return "";
    }
}

// Try-catch-finally block
function readFile(filename: string): string {
    let content: string = "";
    let fileHandle: any = null;

    try {
        // Simulate file opening
        fileHandle = {name: filename, isOpen: true};
        _print("Opening file:", filename);

        // Simulate file reading
        if (filename === "nonexistent.txt") {
            throw new Error("File not found: " + filename);
        }

        content = "File content here";
        _print("File read successfully");

    } catch (error) {
        _print("Error reading file:", error.message);
        content = "";
    } finally {
        // Cleanup code that always runs
        if (fileHandle) {
            fileHandle.isOpen = false;
            _print("File handle closed");
        }
    }

    return content;
}

// Multiple catch blocks (concept - TypeScript doesn't support this directly)
function processData(data: any): any {
    try {
        if (data === null || data === undefined) {
            throw new Error("Data is null or undefined");
        }

        if (typeof data === "string") {
            return JSON.parse(data);
        }

        if (typeof data === "object") {
            return JSON.stringify(data);
        }

        throw new Error("Unsupported data type");

    } catch (error) {
        if (error instanceof SyntaxError) {
            _print("JSON parsing error:", error.message);
            return null;
        } else if (error instanceof TypeError) {
            _print("Type error:", error.message);
            return null;
        } else {
            _print("General error:", error.message);
            return null;
        }
    }
}

// Custom error classes
class ValidationError extends Error {
    field: string;

    constructor(field: string, message: string) {
        super(message);
        this.name = "ValidationError";
        this.field = field;
    }
}

class NetworkError extends Error {
    statusCode: number;

    constructor(message: string, statusCode: number) {
        super(message);
        this.name = "NetworkError";
        this.statusCode = statusCode;
    }
}

class BusinessLogicError extends Error {
    code: string;

    constructor(message: string, code: string) {
        super(message);
        this.name = "BusinessLogicError";
        this.code = code;
    }
}

// Function that throws custom errors
function validateUser(user: any): void {
    if (!user) {
        throw new ValidationError("user", "User object is required");
    }

    if (!user.name || typeof user.name !== "string") {
        throw new ValidationError("name", "User name is required and must be a string");
    }

    if (!user.email || typeof user.email !== "string") {
        throw new ValidationError("email", "User email is required and must be a string");
    }

    if (user.age && (typeof user.age !== "number" || user.age < 0)) {
        throw new ValidationError("age", "User age must be a positive number");
    }
}

// Function that handles custom errors
function createUser(userData: any): object {
    try {
        validateUser(userData);

        return {
            id: Math.floor(Math.random() * 1000),
            name: userData.name,
            email: userData.email,
            age: userData.age || 0,
            createdAt: new Date()
        };

    } catch (error) {
        if (error instanceof ValidationError) {
            _print("Validation failed for field '" + error.field + "': " + error.message);
            return null;
        } else {
            _print("Unexpected error:", error.message);
            return null;
        }
    }
}

// Async error handling
async function fetchUserData(userId: number): Promise<object | null> {
    try {
        // Simulate async operation
        await new Promise(resolve => setTimeout(resolve, 100));

        if (userId <= 0) {
            throw new ValidationError("userId", "User ID must be positive");
        }

        if (userId > 1000) {
            throw new NetworkError("User not found", 404);
        }

        return {
            id: userId,
            name: "User " + userId,
            email: "user" + userId + "@example.com"
        };

    } catch (error) {
        if (error instanceof ValidationError) {
            _print("Validation error:", error.message);
        } else if (error instanceof NetworkError) {
            _print("Network error:", error.message, "Status:", error.statusCode);
        } else {
            _print("Unexpected error:", error.message);
        }
        return null;
    }
}

// Error handling with retries
async function fetchDataWithRetry(url: string, maxRetries: number = 3): Promise<string | null> {
    let lastError: Error | null = null;

    for (let attempt = 1; attempt <= maxRetries; attempt++) {
        try {
            // Simulate network request
            await new Promise(resolve => setTimeout(resolve, 100));

            if (url === "error") {
                throw new NetworkError("Network request failed", 500);
            }

            return "Data from " + url;

        } catch (error) {
            lastError = error;
            _print("Attempt " + attempt + " failed:", error.message);

            if (attempt < maxRetries) {
                // Wait before retry
                await new Promise(resolve => setTimeout(resolve, 1000));
            }
        }
    }

    _print("All retry attempts failed. Last error:", lastError.message);
    return null;
}

// Error handling with cleanup
function processResource(resourceName: string): boolean {
    let resource: any = null;

    try {
        // Acquire resource
        resource = {name: resourceName, isAcquired: true};
        _print("Resource acquired:", resourceName);

        // Process resource
        if (resourceName === "invalid") {
            throw new Error("Invalid resource");
        }

        _print("Resource processed successfully");
        return true;

    } catch (error) {
        _print("Error processing resource:", error.message);
        return false;

    } finally {
        // Always cleanup
        if (resource) {
            resource.isAcquired = false;
            _print("Resource released:", resourceName);
        }
    }
}

// Error handling with logging
function logError(error: Error, context: string): void {
    _print("Error in " + context + ":");
    _print("  Message:", error.message);
    _print("  Name:", error.name);
    _print("  Stack:", error.stack);
}

// Function that uses error logging
function riskyOperation(): number {
    try {
        // Simulate risky operation
        let random = Math.random();
        if (random < 0.3) {
            throw new Error("Random failure occurred");
        }
        return Math.floor(random * 100);

    } catch (error) {
        logError(error, "riskyOperation");
        return -1;
    }
}

// Error handling with fallback values
function getConfigValue(key: string, defaultValue: any): any {
    try {
        // Simulate config lookup
        let config: any = {
            "apiUrl": "https://api.example.com",
            "timeout": 5000,
            "retries": 3
        };

        if (!(key in config)) {
            throw new Error("Configuration key not found: " + key);
        }

        return config[key];

    } catch (error) {
        _print("Config error:", error.message);
        _print("Using default value:", defaultValue);
        return defaultValue;
    }
}

// Usage examples
let result1: number = divide(10, 2);  // 5
let result2: number = divide(10, 0);  // 0 (error caught)

let processed1: string = processString("hello");     // "HELLO"
let processed2: string = processString("");          // "" (error caught)
let processed3: string = processString("a".repeat(101)); // "" (error caught)

let content1: string = readFile("test.txt");        // "File content here"
let content2: string = readFile("nonexistent.txt"); // "" (error caught)

let data1: any = processData('{"name": "Alice"}');  // {name: "Alice"}
let data2: any = processData("invalid json");       // null (error caught)
let data3: any = processData(42);                   // "42"

// Custom error usage
let user1: object | null = createUser({name: "Alice", email: "alice@example.com"});
let user2: object | null = createUser({name: "Bob"}); // null (validation error)
let user3: object | null = createUser(null); // null (validation error)

// Async error handling usage
async function example() {
    let userData1: object | null = await fetchUserData(123);
    let userData2: object | null = await fetchUserData(-1); // null (validation error)
    let userData3: object | null = await fetchUserData(2000); // null (network error)

    let data1: string | null = await fetchDataWithRetry("https://api.example.com");
    let data2: string | null = await fetchDataWithRetry("error"); // null (all retries failed)
}

// Resource processing usage
let success1: boolean = processResource("valid-resource");   // true
let success2: boolean = processResource("invalid");          // false (error caught)

// Error logging usage
let result: number = riskyOperation();

// Config usage
let apiUrl: string = getConfigValue("apiUrl", "https://default.api.com");
let timeout: number = getConfigValue("timeout", 10000);
let unknownKey: string = getConfigValue("unknown", "default-value");

// This example demonstrates comprehensive error handling functionality