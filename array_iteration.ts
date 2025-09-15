
// Advanced array iteration testing
interface Person {
    name: string;
    age: number;
    city: string;
}

function testArrayIteration() {
    const people: Person[] = [
        { name: "Alice", age: 30, city: "New York" },
        { name: "Bob", age: 25, city: "London" },
        { name: "Charlie", age: 35, city: "Tokyo" },
        { name: "Diana", age: 28, city: "Paris" }
    ];
    
    // Iterate over people
    const names: string[] = [];
    for (const person of people) {
        names.push(person.name);
    }
    
    console.log("Names:", names);
    
    // Filter and iterate
    const youngPeople: Person[] = [];
    for (const person of people) {
        if (person.age < 30) {
            youngPeople.push(person);
        }
    }
    
    console.log("Young people:", youngPeople);
    
    // Transform and iterate
    const ages: number[] = [];
    for (const person of people) {
        ages.push(person.age);
    }
    
    console.log("Ages:", ages);
}

// Test with nested arrays
function testNestedArrays() {
    const matrix = [
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ];
    
    const flattened: number[] = [];
    for (const row of matrix) {
        for (const cell of row) {
            flattened.push(cell);
        }
    }
    
    console.log("Flattened matrix:", flattened);
    
    // Sum each row
    const rowSums: number[] = [];
    for (const row of matrix) {
        let sum = 0;
        for (const cell of row) {
            sum += cell;
        }
        rowSums.push(sum);
    }
    
    console.log("Row sums:", rowSums);
}

// Test with array methods and for...of
function testArrayMethodsWithForOf() {
    const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    
    // Filter even numbers and iterate
    const evenNumbers = numbers.filter(n => n % 2 === 0);
    const doubledEvens: number[] = [];
    
    for (const num of evenNumbers) {
        doubledEvens.push(num * 2);
    }
    
    console.log("Doubled evens:", doubledEvens);
    
    // Map and iterate
    const squares = numbers.map(n => n * n);
    const squareRoots: number[] = [];
    
    for (const square of squares) {
        squareRoots.push(Math.sqrt(square));
    }
    
    console.log("Square roots:", squareRoots);
}

// Run tests
testArrayIteration();
testNestedArrays();
testArrayMethodsWithForOf();
