
// Accessing private member
class TestClass {
    private privateMember: number = 42
    public publicMember: string = "public"
}
let instance = new TestClass()
console.log(instance.privateMember)
