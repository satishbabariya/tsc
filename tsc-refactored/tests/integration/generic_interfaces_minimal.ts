// Minimal generic interfaces
interface Container<T> {
    value: T;

    getValue(): T;

    setValue(value: T): void;
}

interface Pair<T, U> {
    first: T;
    second: U;
}