class Container<T> {
    value: T;
    
    constructor(value: T) {
        this.value = value;
    }
    
    test(): string {
        let temp = this.value;      // temp should have type T (TypeParameterType)
        let str = temp.toString;    // This should resolve toString on TypeParameterType
        return "test";
    }
}

function main(): number {
    let container = new Container<number>(42);
    let result = container.test();
    return 0;
}