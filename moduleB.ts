import { add, subtract } from "./moduleA";

export function multiply(a: number, b: number): number {
    return add(a, b) * 2; // Using imported function
}

export function divide(a: number, b: number): number {
    return subtract(a, b) / 2; // Using imported function
}