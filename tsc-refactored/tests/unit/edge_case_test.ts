// Edge Case Regression Test
// Test edge cases and complex scenarios

// Empty union types
type EmptyUnion = never;
type SingleUnion = string;
type TwoUnion = string | number;
type ManyUnion = string | number | boolean | null | undefined;

// Empty intersection types
type EmptyIntersection = {};
type SingleIntersection = { name: string };
type TwoIntersection = { name: string } & { age: number };
type ManyIntersection = { name: string } & { age: number } & { email: string } & { active: boolean };

// Complex conditional types
type ComplexConditional<T> = T extends string ?
    T extends 'hello' ? 'greeting' : 'string' :
    T extends number ?
        T extends 0 ? 'zero' : 'number' :
        T extends boolean ?
            T extends true ? 'truthy' : 'falsy' :
            'unknown';

// Nested mapped types
type NestedMapped<T> = {
    [K in keyof T]: T[K] extends object ? {
        [P in keyof T[K]]: T[K][P] extends string ? `string_${P}` : T[K][P];
    } : T[K];
};

// Recursive types
type RecursiveType<T> = T extends object ? {
    [K in keyof T]: RecursiveType<T[K]>;
} : T;

// Complex generic constraints
interface ComplexConstraint<T extends Record<string, any>> {
    data: T;
    process: <K extends keyof T>(key: K) => T[K];
    transform: <U>(fn: (value: T) => U) => ComplexConstraint<U>;
}

// Test edge cases
function testEdgeCases() {
    // Test empty types
    const emptyUnion: EmptyUnion = (() => {
        throw new Error('Never');
    })();
    const singleUnion: SingleUnion = 'hello';
    const twoUnion: TwoUnion = 42;
    const manyUnion: ManyUnion = true;

    // Test empty intersections
    const emptyIntersection: EmptyIntersection = {};
    const singleIntersection: SingleIntersection = {name: 'Alice'};
    const twoIntersection: TwoIntersection = {name: 'Bob', age: 30};
    const manyIntersection: ManyIntersection = {
        name: 'Charlie',
        age: 25,
        email: 'charlie@example.com',
        active: true
    };

    // Test complex conditionals
    const greeting: ComplexConditional<'hello'> = 'greeting';
    const stringType: ComplexConditional<'world'> = 'string';
    const zero: ComplexConditional<0> = 'zero';
    const number: ComplexConditional<42> = 'number';
    const truthy: ComplexConditional<true> = 'truthy';
    const falsy: ComplexConditional<false> = 'falsy';
    const unknown: ComplexConditional<null> = 'unknown';

    // Test nested mapped types
    const nestedMapped: NestedMapped<{
        user: { name: string; age: number };
        settings: { theme: string; language: string };
    }> = {
        user: {name: 'string_name', age: 30},
        settings: {theme: 'string_theme', language: 'string_language'}
    };

    // Test recursive types
    const recursive: RecursiveType<{
        level1: {
            level2: {
                level3: string;
            };
        };
    }> = {
        level1: {
            level2: {
                level3: 'deep value'
            }
        }
    };

    // Test complex constraints
    const complexConstraint: ComplexConstraint<{ id: number; name: string }> = {
        data: {id: 1, name: 'Test'},
        process: (key) => this.data[key],
        transform: (fn) => ({
            data: fn(this.data),
            process: (key) => fn(this.data)[key],
            transform: (fn2) => this.transform(fn2)
        })
    };

    console.log('Edge case test completed successfully');
}

testEdgeCases();
console.log("Edge case regression test completed");
