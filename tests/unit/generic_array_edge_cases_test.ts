// Test edge cases for array methods with generic types

// Test empty arrays
class EmptyArrayContainer<T> {
    private empty: T[];

    constructor() {
        this.empty = [];
    }

    testEmptyPush(item: T): number {
        return this.empty.push(item);
    }

    testEmptyPop(): T {
        return this.empty.pop();
    }

    testEmptyLength(): number {
        return this.empty.length;
    }

    testEmptyShift(): T {
        return this.empty.shift();
    }

    testEmptyUnshift(item: T): number {
        return this.empty.unshift(item);
    }
}

// Test single element arrays
class SingleElementContainer<T> {
    private single: T[];

    constructor(item: T) {
        this.single = [item];
    }

    testSinglePush(item: T): number {
        return this.single.push(item);
    }

    testSinglePop(): T {
        return this.single.pop();
    }

    testSingleLength(): number {
        return this.single.length;
    }

    testSingleShift(): T {
        return this.single.shift();
    }
}

// Test array method chaining
class ChainingContainer<T> {
    private chain: T[];

    testChainedOperations(item1: T, item2: T, item3: T): number {
        this.chain.push(item1);
        this.chain.push(item2);
        let popped = this.chain.pop();
        this.chain.unshift(item3);
        return this.chain.length;
    }

    testMultiplePushes(item1: T, item2: T, item3: T): number {
        this.chain.push(item1);
        this.chain.push(item2);
        return this.chain.push(item3);
    }

    testPushPopCycle(item: T): T {
        this.chain.push(item);
        return this.chain.pop();
    }
}

// Test array method with different generic types
class TypeVariationContainer<T> {
    private numbers: number[];
    private strings: string[];
    private generics: T[];

    testNumberArray(num: number): number {
        return this.numbers.push(num);
    }

    testStringArray(str: string): number {
        return this.strings.push(str);
    }

    testGenericArray(item: T): number {
        return this.generics.push(item);
    }

    testNumberLength(): number {
        return this.numbers.length;
    }

    testStringLength(): number {
        return this.strings.length;
    }

    testGenericLength(): number {
        return this.generics.length;
    }
}