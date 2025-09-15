
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
