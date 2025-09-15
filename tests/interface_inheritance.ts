// Interface inheritance
interface BaseEntity {
    id: string | number;
    createdAt: string;
    updatedAt: string;
}

interface User extends BaseEntity {
    name: string;
    email: string;
    age?: number;
}

interface Product extends BaseEntity {
    title: string;
    description: string;
    price: number;
    category: string;
}

// Multiple inheritance
interface Readable {
    read(): string;
}

interface Writable {
    write(data: string): void;
}

interface ReadWriteFile extends Readable, Writable {
    filename: string;
    size: number;
}