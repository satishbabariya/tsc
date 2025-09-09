function testToString<T>(value: T): string {
    return value.toString();  // Direct method call on type parameter
}

function main(): number {
    let result = testToString<number>(42);
    return 0;
}