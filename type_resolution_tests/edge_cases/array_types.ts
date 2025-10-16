// Edge case: array type resolution tests
let numbers: number[] = [1, 2, 3, 4, 5];
let strings: string[] = ["a", "b", "c"];
let mixed: (string | number)[] = [1, "hello", 42, "world"];

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
let firstMixed: string | number = mixed[0];

console.log("Sum:", sum);
console.log("Concat:", concat);
console.log("First number:", firstNum);
console.log("First string:", firstStr);
console.log("First mixed:", firstMixed);