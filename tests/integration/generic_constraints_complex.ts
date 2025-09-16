// Complex generic constraint tests
interface BaseEntity {
    id: string;
    name: string;
}

interface User extends BaseEntity {
    email: string;
    age: number;
}

interface Product extends BaseEntity {
    price: number;
    category: string;
}

// Test class inheritance constraints
interface EntityContainer<T extends BaseEntity> {
    entity: T;

    getId(): string;

    getName(): string;
}

// Test interface inheritance constraints
interface UserContainer<T extends User> {
    user: T;

    getEmail(): string;

    getAge(): number;
}

// Test product constraints
interface ProductContainer<T extends Product> {
    product: T;

    getPrice(): number;

    getCategory(): string;
}

// Test multiple inheritance constraints
interface MultiEntityContainer<T extends BaseEntity, U extends User> {
    entity: T;
    user: U;

    process(): T;
}