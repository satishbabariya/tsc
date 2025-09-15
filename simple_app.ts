// Simple app that imports from simple_math
import { add, multiply } from "./simple_math";

export function calculate(x: number, y: number): number {
    const sum = add(x, y);
    const product = multiply(x, y);
    return sum + product;
}