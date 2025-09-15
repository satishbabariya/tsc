
// Edge cases in destructuring testing
function testDestructuringEdgeCases() {
    // Empty arrays
    const emptyArray: number[] = [];
    const [first = 0, second = 0] = emptyArray;
    console.log("Empty array destructuring:", first, second);
    
    // Empty objects
    const emptyObject = {};
    const { prop = "default" } = emptyObject;
    console.log("Empty object destructuring:", prop);
    
    // Deeply nested structures
    const deeplyNested = {
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
    } = deeplyNested;
    console.log("Deeply nested destructuring:", value);
    
    // Combinations of rest and default values
    const mixedArray = [1, 2, 3, 4, 5];
    const [a = 0, b = 0, ...rest] = mixedArray;
    console.log("Mixed destructuring:", a, b, rest);
    
    // Object with rest and defaults
    const mixedObject = { name: "Alice", age: 30 };
    const { name = "Unknown", age = 0, city = "Unknown", ...otherProps } = mixedObject;
    console.log("Mixed object destructuring:", name, age, city, otherProps);
    
    // Null and undefined handling
    const nullValue = null;
    const undefinedValue = undefined;
    
    // Safe destructuring
    const { prop: nullProp = "default" } = nullValue || {};
    const { prop: undefinedProp = "default" } = undefinedValue || {};
    console.log("Null/undefined handling:", nullProp, undefinedProp);
    
    // Array with holes
    const arrayWithHoles = [1, , 3, , 5];
    const [x, y, z, w, v] = arrayWithHoles;
    console.log("Array with holes:", x, y, z, w, v);
    
    // Object with computed properties
    const key = "dynamicKey";
    const objWithComputed = {
        [key]: "computed value",
        [`nested_${key}`]: "nested computed"
    };
    
    const { [key]: computedValue, [`nested_${key}`]: nestedComputed } = objWithComputed;
    console.log("Computed property destructuring:", computedValue, nestedComputed);
}

// Test with function parameters
function testDestructuringFunctionParameters() {
    // Array destructuring in parameters
    function processArray([first, second, ...rest]: number[]) {
        console.log("Array params:", first, second, rest);
    }
    
    // Object destructuring in parameters
    function processObject({ name, age, city = "Unknown" }: { name: string; age: number; city?: string }) {
        console.log("Object params:", name, age, city);
    }
    
    // Mixed destructuring in parameters
    function processMixed({ 
        data: [first, second], 
        config: { theme, notifications = false } 
    }: { 
        data: number[]; 
        config: { theme: string; notifications?: boolean } 
    }) {
        console.log("Mixed params:", first, second, theme, notifications);
    }
    
    processArray([1, 2, 3, 4, 5]);
    processObject({ name: "Alice", age: 30 });
    processMixed({ 
        data: [10, 20], 
        config: { theme: "dark" } 
    });
}

// Run tests
testDestructuringEdgeCases();
testDestructuringFunctionParameters();
