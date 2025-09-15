
// Generic method with interface constraint
interface Comparable {
    compareTo(other: Comparable): number;
}

interface Serializable {
    serialize(): string;
}

class Person implements Comparable, Serializable {
    name: string;
    age: number;
    
    constructor(name: string, age: number) {
        this.name = name;
        this.age = age;
    }
    
    compareTo(other: Person): number {
        return this.age - other.age;
    }
    
    serialize(): string {
        return JSON.stringify({ name: this.name, age: this.age });
    }
}

class Product implements Comparable {
    id: number;
    price: number;
    
    constructor(id: number, price: number) {
        this.id = id;
        this.price = price;
    }
    
    compareTo(other: Product): number {
        return this.price - other.price;
    }
}

// Generic method with T extends Comparable constraint
function sortItems<T extends Comparable>(items: T[]): T[] {
    return items.sort((a, b) => a.compareTo(b));
}

// Generic method with multiple constraints
function processItem<T extends Comparable & Serializable>(item: T): string {
    const comparison = item.compareTo(item);
    const serialized = item.serialize();
    return `Comparison: ${comparison}, Serialized: ${serialized}`;
}

// Test the constraints
const people: Person[] = [
    new Person("Alice", 30),
    new Person("Bob", 25),
    new Person("Charlie", 35)
];

const products: Product[] = [
    new Product(1, 100),
    new Product(2, 50),
    new Product(3, 200)
];

const sortedPeople = sortItems(people);
const sortedProducts = sortItems(products);

const processedPerson = processItem(people[0]);
console.log("Sorted people:", sortedPeople);
console.log("Sorted products:", sortedProducts);
console.log("Processed person:", processedPerson);
