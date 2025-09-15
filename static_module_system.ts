
// Static Module System Test
// Test static imports/exports (no dynamic imports)

// Static named exports
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

// Static default export
export default class MathUtils {
    static square(x: number): number {
        return x * x;
    }
}

// Static re-export
export { PI as PI_CONSTANT, add as addFunction } from './math';
export * from './geometry';

// Type-only exports (compile-time only)
export type { Point, Line } from './geometry';
export type { Calculator as CalculatorType } from './math';

// Static imports (compile-time resolution)
import { PI, E, add, Calculator } from './math';
import MathUtils from './math';
import * as MathNamespace from './math';
import { PI as PI_ALIAS } from './math';
import type { Point, Line } from './geometry';

// Static module usage
console.log("PI:", PI);
console.log("E:", E);
console.log("Add result:", add(5, 3));

const calc = new Calculator();
console.log("Multiply result:", calc.multiply(4, 6));

const mathUtils = new MathUtils();
console.log("Square result:", MathUtils.square(7));

console.log("Math namespace:", MathNamespace);

console.log("Static Module System test completed successfully!");
