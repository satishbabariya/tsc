// Array Assignment Parser Test
// This test verifies that the parser correctly recognizes array assignment syntax

console.log("=== Array Assignment Parser Test ===");

// Test 1: Basic array assignment
let numbers = [1, 2, 3];
numbers[0] = 42;
console.log("Basic assignment: " + numbers[0]);

// Test 2: Array assignment with variables
let index = 1;
let value = 99;
numbers[index] = value;
console.log("Variable assignment: " + numbers[1]);

// Test 3: String array assignment
let words = ["hello", "world"];
words[0] = "goodbye";
console.log("String assignment: " + words[0]);

// Test 4: Multiple assignments
let data = [10, 20, 30];
data[0] = 100;
data[1] = 200;
data[2] = 300;
console.log("Multiple assignments: " + data[0] + ", " + data[1] + ", " + data[2]);

console.log("=== Array Assignment Parser Test Complete ===");