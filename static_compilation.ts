
// Static Compilation Test
// Test static compilation features

// Static class with generics
class StaticContainer<T> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    get(index: number): T | undefined {
        return this.items[index];
    }
    
    size(): number {
        return this.items.length;
    }
    
    map<U>(fn: (item: T) => U): StaticContainer<U> {
        const newContainer = new StaticContainer<U>();
        for (const item of this.items) {
            newContainer.add(fn(item));
        }
        return newContainer;
    }
    
    filter(predicate: (item: T) => boolean): StaticContainer<T> {
        const newContainer = new StaticContainer<T>();
        for (const item of this.items) {
            if (predicate(item)) {
                newContainer.add(item);
            }
        }
        return newContainer;
    }
}

// Static function with generics
function staticProcess<T, U>(input: T, processor: (value: T) => U): U {
    return processor(input);
}

// Static interface with generics
interface StaticProcessor<T, U> {
    process(input: T): U;
    validate(input: T): boolean;
}

// Static implementation
class StringProcessor implements StaticProcessor<string, number> {
    process(input: string): number {
        return input.length;
    }
    
    validate(input: string): boolean {
        return typeof input === 'string' && input.length > 0;
    }
}

// Test static compilation
const stringContainer = new StaticContainer<string>();
stringContainer.add("hello");
stringContainer.add("world");
stringContainer.add("typescript");

console.log("Container size:", stringContainer.size());
console.log("First item:", stringContainer.get(0));

const lengthContainer = stringContainer.map(str => str.length);
console.log("Length container:", lengthContainer);

const filteredContainer = stringContainer.filter(str => str.length > 5);
console.log("Filtered container:", filteredContainer);

const processor = new StringProcessor();
const result = staticProcess("hello world", processor.process);
console.log("Processed result:", result);

const isValid = processor.validate("hello");
console.log("Is valid:", isValid);

console.log("Static Compilation test completed successfully!");
