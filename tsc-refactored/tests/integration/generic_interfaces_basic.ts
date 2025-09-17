// Basic generic interfaces
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

    transform<U>(fn: (value: T) => U): Identity<U>;
}