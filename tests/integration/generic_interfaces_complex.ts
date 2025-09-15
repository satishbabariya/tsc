// Complex generic interfaces
interface Node<T> {
    value: T;
    children: Node<T>[];
    parent?: Node<T>;
    addChild(child: Node<T>): void;
}

interface Tree<T> {
    root: Node<T>;
    find(value: T): Node<T> | null;
    insert(value: T): Node<T>;
    remove(value: T): boolean;
}

interface Cache<K, V> {
    get(key: K): V | null;
    set(key: K, value: V): void;
    delete(key: K): boolean;
    clear(): void;
    size(): number;
}

interface EventEmitter<T extends Record<string, any>> {
    on<K extends keyof T>(event: K, listener: (data: T[K]) => void): void;
    emit<K extends keyof T>(event: K, data: T[K]): void;
    off<K extends keyof T>(event: K, listener: (data: T[K]) => void): void;
}