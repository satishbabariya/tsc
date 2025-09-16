// Type error that affects code generation
function test(x: number): number {
    return x * 2
}

let result = test("hello") // Type error affects code generation
