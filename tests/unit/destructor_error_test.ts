// Test file for destructor syntax error handling

// Test 1: Wrong class name in destructor
class TestClass {
~

    WrongClassName() {  // Should error: class name mismatch
        _print("Wrong destructor name");
    }
}

// Test 2: Destructor with parameters (should not be allowed)
class ParameterTest {
~

    ParameterTest(param: string) {  // Should error: destructors take no parameters
        _print("Destructor with parameters");
    }
}

// Test 3: Multiple destructors in same class
class MultipleDestructors {
~

    MultipleDestructors() {
        _print("First destructor");
    }

~

    MultipleDestructors() {  // Should error: multiple destructors
        _print("Second destructor");
    }
}