// Basic interface tests
interface User {
    id: string;
    name: string;
    age: number;
}

interface Product {
    id: number;
    name: string;
    price: number;
    inStock: boolean;
}

// Interface with union types
interface Config {
    mode: "development" | "production" | "test";
    timeout: number | null;
    debug: boolean | string;
}