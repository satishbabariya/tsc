// Integration tests for generic interfaces with properties only
interface DataContainer<T> {
    data: T;
    id: string;
    timestamp: number;
}

interface Processor<T> {
    item: T;
    valid: boolean;
}

interface Cache<T> {
    key: string;
    value: T;
    size: number;
}

// Test generic interfaces with array types
interface ArrayContainer<T> {
    items: T[];
    length: number;
}

// Test generic interfaces with literal types
interface StatefulContainer<T> {
    value: T;
    state: "initialized" | "processing" | "complete";
    priority: 1 | 2 | 3 | 4 | 5;
}