// Simple test for Array<T> in function signatures
function processArrayT<T>(arr: Array<T>): number {
    return arr.length;
}

function processArrayBracket<T>(arr: T[]): number {
    return arr.length;
}

let testArrayT: Array<number> = [];
testArrayT.push(1);
_print("Function with Array<T> parameter:", processArrayT(testArrayT));

let testArrayBracket: number[] = [];
testArrayBracket.push(2);
_print("Function with T[] parameter:", processArrayBracket(testArrayBracket));