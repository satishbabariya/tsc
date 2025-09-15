
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
