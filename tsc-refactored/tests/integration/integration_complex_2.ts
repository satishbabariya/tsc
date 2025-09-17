// Error in class context
class TestClass {
    private member: number = "hello" // Type error
    public method(): void {
        console.log(undefinedVariable) // Semantic error
        this.member = "world" // Type error
    }
}
