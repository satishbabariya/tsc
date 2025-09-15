// simple_test.ts - Simple test without imports
export function add(a: number, b: number): number {
    return a + b;
}

export function subtract(a: number, b: number): number {
    return a - b;
}

function main(): number {
    return add(5, 3) + subtract(10, 4);
}