
// Complex type relationships for type checker coverage
interface GenericInterface<T> {
    value: T;
    method(): T;
}

class GenericClass<T> implements GenericInterface<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    method(): T {
        return this.value;
    }
}

function complexTypeRelationships(): void {
    // Generic type instantiation
    let stringGeneric = new GenericClass<string>("hello");
    let numberGeneric = new GenericClass<number>(42);
    let booleanGeneric = new GenericClass<boolean>(true);
    
    // Complex type unions
    let complexUnion: string | number | boolean = "hello";
    complexUnion = 42;
    complexUnion = true;
    
    // Complex type intersections
    let complexIntersection: GenericInterface<string> & { additional: number } = {
        value: "test",
        method: () => "test",
        additional: 42
    };
    
    // Type parameter constraints
    function constrainedFunction<T extends string | number>(value: T): T {
        return value;
    }
    
    let constrainedString = constrainedFunction<string>("hello");
    let constrainedNumber = constrainedFunction<number>(42);
    
    console.log(stringGeneric, numberGeneric, booleanGeneric, complexUnion, complexIntersection, constrainedString, constrainedNumber);
}
