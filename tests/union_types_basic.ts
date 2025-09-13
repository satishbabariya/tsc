// Basic union type tests
let stringOrNumber: string | number;
let booleanOrNull: boolean | null;
let tripleUnion: string | number | boolean;
let primitiveUnion: string | number | boolean | null;

// Function with union parameters
function processUnion(param: string | number): string {
    return String(param);
}

// Function returning union type
function getValue(): string | number {
    return Math.random() > 0.5 ? "hello" : 42;
}

// Class with union properties
class TestClass {
    id: string | number;
    active: boolean | null;
    value: string | number | boolean;
}