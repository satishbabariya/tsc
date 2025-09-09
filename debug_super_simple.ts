// Very simple test for super method calls

class Derived extends Base {
    test(): number {
        return super.getValue();
    }
}

class Base {
    getValue(): number {
        return 42;
    }
}

function main(): number {
    return 0;
}