// Semantic Error Test Case
function semanticErrorTest(): void {
    let declaredVariable: number = 42;
    let anotherVariable: string = "test";

    // Undefined variable references
    console.log(declaredVariable);
    console.log(anotherVariable);
    console.log(undeclaredVariable);
    console.log(anotherUndeclaredVariable);

    // Duplicate declarations
    let duplicateVar: number = 10;
    let duplicateVar: string = "duplicate";

    // Function before declaration
    let result = useBeforeDeclared();

    function useBeforeDeclared(): number {
        return 42;
    }

    // Class member access errors
    class TestClass {
        public publicMember: string = "public";
        private privateMember: number = 42;
    }

    let instance = new TestClass();
    let privateAccess = instance.privateMember;
    let publicAccess = instance.publicMember;
}
