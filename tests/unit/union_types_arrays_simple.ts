// Simple array union type tests
let numberOrStringArray: number[] | string[];
let userArrayOrNull: User[] | null;
let matrix: number[][] | string[][];

class User {
    name: string;
    age: number;
}

// Function with array union parameters
function processArray(arr: number[] | string[]): number {
    return arr.length;
}

function handleUserList(users: User[] | null): void {
    // Process users
}

// Class with array union properties
class DataContainer {
    items: number[] | string[];
    users: User[] | null;
    matrix: number[][] | string[][];
}