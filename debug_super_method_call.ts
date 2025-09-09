// Test case to isolate super method call issue

class Base {
    getValue(): number {
        return 42;
    }
}

class Derived extends Base {
    getValue(): number {
        // This should work but fails with "Expression is not callable"
        return super.getValue();
    }
}

function main(): number {
    let d = new Derived();
    return 0;
}