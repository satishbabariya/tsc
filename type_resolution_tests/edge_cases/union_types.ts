// Edge case: union type resolution tests
type StringOrNumber = string | number;

function processValue(value: StringOrNumber): string {
    if (typeof value === "string") {
        return "String: " + value;
    } else {
        return "Number: " + value.toString();
    }
}

// Test: union type handling
let val1: StringOrNumber = "hello";
let val2: StringOrNumber = 42;

let result1: string = processValue(val1);
let result2: string = processValue(val2);

console.log("Result 1:", result1);
console.log("Result 2:", result2);