// Simplified array type tests (without generic syntax)
let numbers: number[] = [1, 2, 3, 4, 5];
let strings: string[] = ["a", "b", "c"];

// Test: array method calls
let sum: number = 0;
for (let i = 0; i < numbers.length; i++) {
    sum += numbers[i];
}

let concat: string = "";
for (let i = 0; i < strings.length; i++) {
    concat += strings[i];
}

// Test: array access
let firstNum: number = numbers[0];
let firstStr: string = strings[0];

console.log("Sum:", sum);
console.log("Concat:", concat);
console.log("First number:", firstNum);
console.log("First string:", firstStr);