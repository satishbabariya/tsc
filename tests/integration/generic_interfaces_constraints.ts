// Generic interfaces with constraints
interface Comparable {
    compare(other: Comparable): number;
}

interface ComparableContainer<T extends Comparable> {
    value: T;
    compare(other: ComparableContainer<T>): number;
}

interface Serializable {
    serialize(): string;
}

interface SerializableContainer<T extends Serializable> {
    value: T;
    serialize(): string;
}