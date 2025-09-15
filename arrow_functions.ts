
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
