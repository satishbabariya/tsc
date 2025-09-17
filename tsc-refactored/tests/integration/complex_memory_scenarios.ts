// Complex memory scenarios
function advancedMemoryStress(): void {
    // Large nested object structures
    let largeObject = {};
    for (let i = 0; i < 1000; i++) {
        largeObject[`level1_${i}`] = {};
        for (let j = 0; j < 100; j++) {
            largeObject[`level1_${i}`][`level2_${j}`] = {};
            for (let k = 0; k < 10; k++) {
                largeObject[`level1_${i}`][`level2_${j}`][`level3_${k}`] = {
                    data: new Array(1000).fill(i * j * k),
                    metadata: {
                        created: new Date(),
                        id: `${i}-${j}-${k}`,
                        nested: {
                            deep: {
                                deeper: {
                                    deepest: new Array(100).fill("deep data")
                                }
                            }
                        }
                    }
                };
            }
        }
    }

    // Large array operations
    let largeArrays = [];
    for (let i = 0; i < 100; i++) {
        let array = new Array(10000);
        for (let j = 0; j < 10000; j++) {
            array[j] = {
                index: j,
                value: Math.random() * 1000,
                nested: {
                    data: new Array(100).fill(j),
                    metadata: new Array(50).fill(`item_${j}`)
                }
            };
        }
        largeArrays.push(array);
    }

    // Complex function closures
    let closures = [];
    for (let i = 0; i < 1000; i++) {
        let closure = (function (index) {
            let data = new Array(1000).fill(index);
            return function () {
                return data.map(x => x * Math.random());
            };
        })(i);
        closures.push(closure);
    }

    // Execute closures to create memory pressure
    closures.forEach(closure => {
        try {
            closure();
        } catch (error) {
            // Handle potential memory errors
        }
    });
}

// Memory leak simulation
function memoryLeakSimulation(): void {
    let leakedObjects = [];

    // Simulate memory leaks
    for (let i = 0; i < 10000; i++) {
        let leakedObject = {
            id: i,
            data: new Array(1000).fill(i),
            circular: null as any
        };

        // Create circular reference
        leakedObject.circular = leakedObject;

        leakedObjects.push(leakedObject);
    }

    // Don't clean up - simulate memory leak
    console.log("Memory leak simulation complete");
}
