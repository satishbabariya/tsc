// Complex integration tests combining multiple generic interfaces
interface Repository<T> {
    find(id: string): T | null;
    save(item: T): void;
    delete(id: string): boolean;
    list(): T[];
}

interface Service<T> {
    repository: Repository<T>;
    process(item: T): T;
    validate(item: T): boolean;
}

interface Controller<T> {
    service: Service<T>;
    handle(request: T): T;
    authenticate(request: T): boolean;
}

// Test multiple generic interfaces in same scope
interface User {
    id: string;
    name: string;
    email: string;
}

interface Product {
    id: number;
    name: string;
    price: number;
}

interface Order {
    id: string;
    userId: string;
    productIds: number[];
    total: number;
}

// Generic interfaces that reference each other
interface Node<T> {
    value: T;
    parent: Node<T> | null;
    children: Node<T>[];
}

interface Tree<T> {
    root: Node<T> | null;
    size: number;
    insert(value: T): Node<T>;
    find(value: T): Node<T> | null;
}