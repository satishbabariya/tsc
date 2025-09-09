// Test if type parameter T is accessible in class scope
class Container<T> {
    // Try to use T in a simple way to see if it's resolved
    value: T;  // This should resolve T to TypeParameterType
}

function main(): number {
    return 0;
}