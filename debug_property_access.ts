// Test property access on generic type parameter
class Container<T> {
    value: T;
    
    test(): T {
        let temp = this.value;  // This works
        temp.toString;          // This should trigger the error
        return temp;            
    }
}

function main(): number {
    return 0;
}