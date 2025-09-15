// Simple debug test to understand the parsing issue

// This works - simple constraint
function test1<T extends string>(value: T): T {
    return value;
}

// This fails - union constraint
function test2<T extends string | number>(value: T): T {
    return value;
}