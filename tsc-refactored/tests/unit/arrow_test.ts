// Arrow function test
const add = (a: number, b: number): number => a + b;

const multiply = (x: number, y: number): number => {
    return x * y;
};

export function testArrowFunctions(): number {
    return add(5, 3) + multiply(2, 4);
}
