// Integration tests for generic interfaces with other language features
interface DataContainer<T> {
    data: T;
    id: string;
    timestamp: number;
}

interface Processor<T> {
    process(item: T): T;
    validate(item: T): boolean;
}

interface Cache<T> {
    get(key: string): T | null;
    set(key: string, value: T): void;
    clear(): void;
}

// Test generic interfaces with union types
interface FlexibleContainer<T> {
    value: T | null;
    metadata: string | number;
    status: "pending" | "completed" | "failed";
}

// Test generic interfaces with array types
interface ArrayContainer<T> {
    items: T[];
    length: number;
    add(item: T): void;
    get(index: number): T | null;
}

// Test generic interfaces with literal types
interface StatefulContainer<T> {
    value: T;
    state: "initialized" | "processing" | "complete";
    priority: 1 | 2 | 3 | 4 | 5;
}