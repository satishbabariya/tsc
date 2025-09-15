
// Complex type inference for type checker coverage
function complexTypeInference(): void {
    // Complex array type inference
    let complexArray = [
        [1, 2, 3],
        ["a", "b", "c"],
        [true, false, true]
    ];
    
    // Complex object type inference
    let complexObject = {
        nested: {
            deeply: {
                nested: {
                    value: 42,
                    array: [1, 2, 3],
                    object: { a: 1, b: 2 }
                }
            }
        }
    };
    
    // Complex function type inference
    let complexFunction = (a: number, b: string) => {
        return { number: a, string: b, combined: `${a}:${b}` };
    };
    
    // Complex conditional type inference
    let conditionalType = Math.random() > 0.5 ? "string" : 42;
    
    // Complex mapped types
    let mappedObject = {
        a: 1,
        b: 2,
        c: 3
    };
    
    let mappedResult = Object.keys(mappedObject).map(key => mappedObject[key] * 2);
    
    console.log(complexArray, complexObject, complexFunction, conditionalType, mappedResult);
}
