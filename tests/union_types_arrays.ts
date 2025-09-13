// Array union type tests
let numberOrStringArray: number[] | string[];
let userArrayOrNull: User[] | null;
let mixedArrays: (string | number)[];

class User {
    name: string;
    age: number;
}

// Multi-dimensional array unions
let matrix: number[][] | string[][];
let complexArray: (User | null)[];

// Function with array union parameters
function processArray(arr: number[] | string[]): number {
    return arr.length;
}

function handleUserList(users: User[] | null): void {
    if (users !== null) {
        // Process users
    }
}

// Class with array union properties
class DataContainer {
    items: (string | number)[];
    users: User[] | null;
    matrix: number[][] | string[][];
}