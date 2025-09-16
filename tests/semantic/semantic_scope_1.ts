// Accessing private member
class TestClass {
    public publicMember: string = "public"
    private privateMember: number = 42
}

let instance = new TestClass()
console.log(instance.privateMember)
