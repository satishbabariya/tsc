// Simple generic interfaces without complex syntax
interface Container<T> {
    value: T;

    getValue(): T;

    setValue(value: T): void;
}

interface Pair<T, U> {
    first: T;
    second: U;

    swap(): Pair<U, T>;
}

interface Identity<T> {
    value: T;

    transform(fn: (value: T) => T): Identity<T>;
}