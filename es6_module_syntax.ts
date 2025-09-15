
// ES6 Module Syntax Test
// Test import/export functionality

// Named exports
export const PI = 3.14159;
export const E = 2.71828;
export function add(a: number, b: number): number {
    return a + b;
}
export class Calculator {
    multiply(a: number, b: number): number {
        return a * b;
    }
}

// Default export
export default class MathUtils {
    static square(x: number): number {
        return x * x;
    }
}

// Re-export
export { PI as PI_CONSTANT, add as addFunction } from './math';
export * from './geometry';

// Type-only exports
export type { Point, Line } from './geometry';
export type { Calculator as CalculatorType } from './math';

// Import tests
import { PI, E, add, Calculator } from './math';
import MathUtils from './math';
import * as MathNamespace from './math';
import { PI as PI_ALIAS } from './math';
import type { Point, Line } from './geometry';

// Dynamic import
async function loadModule() {
    const module = await import('./dynamic-module');
    return module.default;
}

// Test module usage
console.log("PI:", PI);
console.log("E:", E);
console.log("Add result:", add(5, 3));

const calc = new Calculator();
console.log("Multiply result:", calc.multiply(4, 6));

const mathUtils = new MathUtils();
console.log("Square result:", MathUtils.square(7));

console.log("Math namespace:", MathNamespace);

// Test dynamic import
loadModule().then(module => {
    console.log("Dynamic module loaded:", module);
});

console.log("ES6 Module Syntax test completed successfully!");
