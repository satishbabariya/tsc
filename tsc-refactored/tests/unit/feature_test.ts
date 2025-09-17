// Testing various implemented features
function testImplementedFeatures(): void {
    // Variables and types
    let num: number = 42;
    let str: string = "hello";
    let bool: boolean = true;

    // Control flow with comparisons (recently fixed!)
    if (num < 50) {
        num = num + 1;
    }

    // Arrays and objects
    let arr = [1, 2, 3];
    let obj = {x: 10, y: 20};
    let value = arr[0] + obj.x;

    // Functions with generics (implemented!)
    function identity<T>(x: T): T {
        return x;
    }

    let result = identity<number>(value);
}

// Classes with inheritance (fully working!)
class Animal {
    name: string;

    constructor(name: string) {
        this.name = name;
    }

    speak(): string {
        return "Some sound";
    }
}

class Dog extends Animal {
    breed: string;

    constructor(name: string, breed: string) {
        super(name);
        this.breed = breed;
    }

    speak(): string {
        return "Woof!";
    }

    wagTail(): void {
        // Method specific to Dog
    }
}

// Interfaces (working!)
interface Flyable {
    fly(): void;
}

// Enums (working!)
enum Color {
    Red = 1,
    Green = 2,
    Blue = 3
}

// Type aliases (working!)
type Point = { x: number; y: number; };

function main(): number {
    let dog = new Dog("Buddy", "Golden Retriever");
    let sound = dog.speak();

    let color = Color.Red;
    let point: Point = {x: 1, y: 2};

    return 0;
}