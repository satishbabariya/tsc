// Generic interfaces with inheritance
interface BaseContainer<T> {
    value: T;
    getValue(): T;
}

interface ReadableContainer<T> extends BaseContainer<T> {
    read(): T;
}

interface WritableContainer<T> extends BaseContainer<T> {
    write(value: T): void;
}

interface ReadWriteContainer<T> extends ReadableContainer<T>, WritableContainer<T> {
    clear(): void;
}