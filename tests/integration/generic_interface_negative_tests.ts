// Negative test cases for generic interfaces - these should fail with proper error messages

// Test 1: Missing type parameter name
interface MissingTypeParam<> {
    value: string;
}

// Test 2: Duplicate type parameter names
interface DuplicateParams<T, T> {
    value: T;
}

// Test 3: Invalid type parameter syntax
interface InvalidSyntax<T, U, V> {
    value: T;
}

// Test 4: Missing closing bracket
interface MissingClosing<T {
    value: T;
}

// Test 5: Extra comma
interface ExtraComma<T,> {
    value: T;
}