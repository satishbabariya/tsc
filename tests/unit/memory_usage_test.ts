// Memory Usage Performance Test
// Test memory usage with complex type systems

// Large type definitions
interface LargeType {
    id: number;
    name: string;
    description: string;
    metadata: {
        created: Date;
        updated: Date;
        version: string;
        tags: string[];
        properties: Record<string, any>;
    };
    data: {
        values: number[];
        strings: string[];
        objects: Array<{
            key: string;
            value: any;
            nested: {
                deep: {
                    value: any;
                };
            };
        }>;
    };
}

// Generic type with large data
class LargeGenericType<T> {
    private data: T[] = [];
    private cache = new Map<string, T>();
    private metadata = new Map<string, any>();

    add(item: T): void {
        this.data.push(item);
    }

    get(index: number): T | undefined {
        return this.data[index];
    }

    setCache(key: string, value: T): void {
        this.cache.set(key, value);
    }

    getCache(key: string): T | undefined {
        return this.cache.get(key);
    }

    setMetadata(key: string, value: any): void {
        this.metadata.set(key, value);
    }

    getMetadata(key: string): any {
        return this.metadata.get(key);
    }

    clear(): void {
        this.data = [];
        this.cache.clear();
        this.metadata.clear();
    }
}

// Memory usage test
function memoryUsageTest() {
    const startMemory = process.memoryUsage();

    const largeTypes: LargeGenericType<LargeType>[] = [];

    // Create many large types
    for (let i = 0; i < 1000; i++) {
        const largeType = new LargeGenericType<LargeType>();

        // Add large data
        for (let j = 0; j < 100; j++) {
            largeType.add({
                id: j,
                name: `Item ${j}`,
                description: `Description for item ${j}`,
                metadata: {
                    created: new Date(),
                    updated: new Date(),
                    version: '1.0.0',
                    tags: [`tag${j}`, `category${j}`],
                    properties: {
                        prop1: `value${j}`,
                        prop2: j * 2,
                        prop3: {nested: {value: j}}
                    }
                },
                data: {
                    values: Array.from({length: 100}, (_, k) => k + j),
                    strings: Array.from({length: 50}, (_, k) => `string${k}`),
                    objects: Array.from({length: 25}, (_, k) => ({
                        key: `key${k}`,
                        value: k,
                        nested: {
                            deep: {
                                value: k * 2
                            }
                        }
                    }))
                }
            });
        }

        largeTypes.push(largeType);
    }

    const endMemory = process.memoryUsage();
    const memoryDelta = endMemory.heapUsed - startMemory.heapUsed;

    console.log(`Memory usage test completed`);
    console.log(`Start memory: ${startMemory.heapUsed} bytes`);
    console.log(`End memory: ${endMemory.heapUsed} bytes`);
    console.log(`Memory delta: ${memoryDelta} bytes`);
    console.log(`Memory per item: ${memoryDelta / 1000} bytes`);

    // Clear memory
    largeTypes.forEach(type => type.clear());
}

memoryUsageTest();
console.log("Memory usage performance test completed");
