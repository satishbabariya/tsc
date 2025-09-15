
// Wrong argument types
function test(a: number, b: string): void {
    console.log(a, b)
}
test("hello", 42)
