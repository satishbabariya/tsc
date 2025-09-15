
// Deep Nesting Performance Test
// Test deeply nested generics and complex type inference

// Level 1: Basic generic
interface Container<T> {
    value: T;
    next?: Container<T>;
}

// Level 2: Generic with constraint
interface ConstrainedContainer<T extends string | number> extends Container<T> {
    validator: (value: T) => boolean;
}

// Level 3: Generic with multiple type parameters
interface MultiContainer<T, U, V> extends ConstrainedContainer<T> {
    second: U;
    third: V;
    processor: (t: T, u: U, v: V) => T;
}

// Level 4: Generic with conditional types
type ConditionalContainer<T> = T extends string ? 
    MultiContainer<T, number, boolean> : 
    MultiContainer<T, string, number>;

// Level 5: Generic with mapped types
type MappedContainer<T> = {
    [K in keyof T]: ConditionalContainer<T[K]>;
};

// Level 6: Generic with recursive types
type RecursiveContainer<T> = T extends object ? {
    [K in keyof T]: RecursiveContainer<T[K]>;
} : ConditionalContainer<T>;

// Level 7: Generic with utility types
type UtilityContainer<T> = Partial<Required<Readonly<RecursiveContainer<T>>>>;

// Level 8: Generic with intersection types
type IntersectionContainer<T> = UtilityContainer<T> & {
    metadata: {
        created: Date;
        updated: Date;
        version: string;
    };
};

// Level 9: Generic with union types
type UnionContainer<T> = IntersectionContainer<T> | {
    fallback: T;
    error: string;
};

// Level 10: Final complex generic
type ComplexContainer<T> = UnionContainer<T> & {
    transform: <U>(fn: (value: T) => U) => ComplexContainer<U>;
    map: <U>(fn: (value: T) => U) => ComplexContainer<U>;
    filter: (fn: (value: T) => boolean) => ComplexContainer<T>;
    reduce: <U>(fn: (acc: U, value: T) => U, initial: U) => U;
};

// Test function with deep nesting
function createDeepNestedContainer<T>(value: T): ComplexContainer<T> {
    return {
        value,
        validator: (v: T) => !!v,
        second: 'test',
        third: 42,
        processor: (t: T, u: string, v: number) => t,
        metadata: {
            created: new Date(),
            updated: new Date(),
            version: '1.0.0'
        },
        transform: <U>(fn: (value: T) => U) => createDeepNestedContainer(fn(value)),
        map: <U>(fn: (value: T) => U) => createDeepNestedContainer(fn(value)),
        filter: (fn: (value: T) => boolean) => fn(value) ? createDeepNestedContainer(value) : createDeepNestedContainer(value),
        reduce: <U>(fn: (acc: U, value: T) => U, initial: U) => fn(initial, value)
    };
}

// Performance test
function performanceTest() {
    const startTime = performance.now();
    
    // Create many nested containers
    for (let i = 0; i < 1000; i++) {
        const container = createDeepNestedContainer(i);
        const transformed = container.transform(x => x * 2);
        const mapped = container.map(x => x.toString());
        const filtered = container.filter(x => x > 0);
        const reduced = container.reduce((acc, x) => acc + x, 0);
    }
    
    const endTime = performance.now();
    console.log(`Deep nesting test completed in ${endTime - startTime}ms`);
}

performanceTest();
console.log("Deep nesting performance test completed");
