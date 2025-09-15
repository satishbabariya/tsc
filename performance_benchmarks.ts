
// Performance benchmarking testing
function testPerformanceBenchmarks() {
    // Test array destructuring performance
    const largeArray = Array.from({ length: 10000 }, (_, i) => i);
    
    console.time("Array destructuring");
    for (let i = 0; i < 1000; i++) {
        const [first, second, ...rest] = largeArray;
    }
    console.timeEnd("Array destructuring");
    
    // Test object destructuring performance
    const largeObject = Object.fromEntries(
        Array.from({ length: 1000 }, (_, i) => [`key${i}`, i])
    );
    
    console.time("Object destructuring");
    for (let i = 0; i < 1000; i++) {
        const { key0, key1, ...rest } = largeObject;
    }
    console.timeEnd("Object destructuring");
    
    // Test nested destructuring performance
    const nestedData = {
        level1: {
            level2: {
                level3: {
                    level4: {
                        level5: {
                            value: "deep value"
                        }
                    }
                }
            }
        }
    };
    
    console.time("Nested destructuring");
    for (let i = 0; i < 1000; i++) {
        const { 
            level1: { 
                level2: { 
                    level3: { 
                        level4: { 
                            level5: { value } 
                        } 
                    } 
                } 
            } 
        } = nestedData;
    }
    console.timeEnd("Nested destructuring");
    
    // Test object literal creation performance
    console.time("Object literal creation");
    for (let i = 0; i < 1000; i++) {
        const obj = {
            name: "Alice",
            age: 30,
            city: "New York",
            country: "USA",
            email: "alice@example.com"
        };
    }
    console.timeEnd("Object literal creation");
    
    // Test property access performance
    const testObject = {
        name: "Alice",
        age: 30,
        city: "New York",
        country: "USA",
        email: "alice@example.com"
    };
    
    console.time("Property access");
    for (let i = 0; i < 1000; i++) {
        const name = testObject.name;
        const age = testObject.age;
        const city = testObject.city;
    }
    console.timeEnd("Property access");
    
    // Test method call performance
    const testObjectWithMethods = {
        name: "Alice",
        age: 30,
        getName(): string {
            return this.name;
        },
        getAge(): number {
            return this.age;
        },
        getInfo(): string {
            return `${this.name} is ${this.age} years old`;
        }
    };
    
    console.time("Method calls");
    for (let i = 0; i < 1000; i++) {
        const name = testObjectWithMethods.getName();
        const age = testObjectWithMethods.getAge();
        const info = testObjectWithMethods.getInfo();
    }
    console.timeEnd("Method calls");
}

// Test with large datasets
function testLargeDatasetPerformance() {
    // Test with very large arrays
    const veryLargeArray = Array.from({ length: 100000 }, (_, i) => i);
    
    console.time("Large array destructuring");
    const [first, second, ...rest] = veryLargeArray;
    console.timeEnd("Large array destructuring");
    
    // Test with very large objects
    const veryLargeObject = Object.fromEntries(
        Array.from({ length: 10000 }, (_, i) => [`key${i}`, i])
    );
    
    console.time("Large object destructuring");
    const { key0, key1, key2, ...rest } = veryLargeObject;
    console.timeEnd("Large object destructuring");
    
    // Test with deeply nested objects
    const deeplyNestedObject = {
        level1: {
            level2: {
                level3: {
                    level4: {
                        level5: {
                            level6: {
                                level7: {
                                    level8: {
                                        level9: {
                                            level10: {
                                                value: "very deep value"
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };
    
    console.time("Deeply nested destructuring");
    const { 
        level1: { 
            level2: { 
                level3: { 
                    level4: { 
                        level5: { 
                            level6: { 
                                level7: { 
                                    level8: { 
                                        level9: { 
                                            level10: { value } 
                                        } 
                                    } 
                                } 
                            } 
                        } 
                    } 
                } 
            } 
        } 
    } = deeplyNestedObject;
    console.timeEnd("Deeply nested destructuring");
}

// Test with complex operations
function testComplexOperationPerformance() {
    // Test with complex destructuring operations
    const complexData = {
        users: Array.from({ length: 1000 }, (_, i) => ({
            id: i,
            name: `User${i}`,
            age: 20 + (i % 50),
            city: `City${i % 10}`
        })),
        settings: {
            theme: "dark",
            language: "en",
            notifications: true
        },
        metadata: {
            version: "1.0.0",
            timestamp: Date.now()
        }
    };
    
    console.time("Complex destructuring");
    for (let i = 0; i < 100; i++) {
        const { 
            users: [firstUser, secondUser, ...otherUsers],
            settings: { theme, language },
            metadata: { version }
        } = complexData;
    }
    console.timeEnd("Complex destructuring");
    
    // Test with complex object operations
    console.time("Complex object operations");
    for (let i = 0; i < 100; i++) {
        const obj = {
            name: "Alice",
            age: 30,
            profile: {
                address: {
                    street: "123 Main St",
                    city: "New York",
                    country: "USA"
                },
                preferences: {
                    theme: "dark",
                    notifications: true
                }
            },
            getInfo(): string {
                return `${this.name} is ${this.age} years old`;
            }
        };
        
        const info = obj.getInfo();
        const city = obj.profile.address.city;
        const theme = obj.profile.preferences.theme;
    }
    console.timeEnd("Complex object operations");
}

// Run tests
testPerformanceBenchmarks();
testLargeDatasetPerformance();
testComplexOperationPerformance();
