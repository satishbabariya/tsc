// Control Flow Statements
// Demonstrates if/else, loops, and conditional execution

// If-else statements
let age: number = 18;
let hasLicense: boolean = true;

if (age >= 18) {
    // This block executes
    let message: string = "You are an adult";
} else {
    let message: string = "You are a minor";
}

// Nested if-else
if (age >= 18) {
    if (hasLicense) {
        let message: string = "You can drive";
    } else {
        let message: string = "You need a license to drive";
    }
} else {
    let message: string = "You are too young to drive";
}

// While loops
let count: number = 0;
while (count < 5) {
    count++;
}

// Do-while loops
let attempts: number = 0;
do {
    attempts++;
} while (attempts < 3);

// For loops
for (let i: number = 0; i < 10; i++) {
    // Loop body
}

// For loops with multiple variables
for (let i: number = 0, j: number = 10; i < j; i++, j--) {
    // Loop body
}

// For-in loops (iterate over object properties)
let person: object = { name: "John", age: 30, city: "New York" };
for (let key in person) {
    // Iterate over property names
}

// For-of loops (iterate over iterable values)
let numbers: number[] = [1, 2, 3, 4, 5];
for (let num of numbers) {
    // Iterate over array values
}

// Break and continue
for (let i: number = 0; i < 10; i++) {
    if (i === 5) {
        break;  // Exit the loop
    }
    if (i === 3) {
        continue;  // Skip to next iteration
    }
}

// Switch statements
let day: number = 3;
let dayName: string;

switch (day) {
    case 1:
        dayName = "Monday";
        break;
    case 2:
        dayName = "Tuesday";
        break;
    case 3:
        dayName = "Wednesday";
        break;
    case 4:
        dayName = "Thursday";
        break;
    case 5:
        dayName = "Friday";
        break;
    case 6:
        dayName = "Saturday";
        break;
    case 7:
        dayName = "Sunday";
        break;
    default:
        dayName = "Invalid day";
        break;
}

// Switch with fall-through
let month: number = 2;
let daysInMonth: number;

switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        daysInMonth = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        daysInMonth = 30;
        break;
    case 2:
        daysInMonth = 28;  // Simplified (not considering leap years)
        break;
    default:
        daysInMonth = 0;
        break;
}

// This example demonstrates all control flow constructs