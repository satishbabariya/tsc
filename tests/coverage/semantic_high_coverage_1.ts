// Comprehensive semantic analysis for high coverage
interface TestInterface {
    name: string;
    value: number;

    method(): string;
}

class TestClass implements TestInterface {
    public name: string;
    protected flag: boolean;
    private value: number;

    constructor(name: string, value: number) {
        this.name = name;
        this.value = value;
        this.flag = true;
    }

    public method(): string {
        return this.name + ":" + this.value;
    }

    protected getValue(): number {
        return this.value;
    }

    private helper(): number {
        return this.value * 2;
    }
}

function comprehensiveSemanticAnalysis(): void {
    // Variable scope analysis
    let globalVar = "global";

    function innerFunction() {
        let localVar = "local";
        console.log(globalVar, localVar);

        function nestedFunction() {
            let nestedVar = "nested";
            console.log(globalVar, localVar, nestedVar);
        }

        nestedFunction();
    }

    innerFunction();

    // Class instantiation and method calls
    let instance = new TestClass("test", 42);
    console.log(instance.method());

    // Interface implementation
    let interfaceImpl: TestInterface = instance;
    console.log(interfaceImpl.name, interfaceImpl.value);

    // Array and object operations
    let arr = [1, 2, 3, 4, 5];
    arr.push(6);
    arr.pop();
    arr.shift();
    arr.unshift(0);

    let obj = {a: 1, b: 2, c: 3};
    obj.d = 4;
    delete obj.c;

    console.log(arr, obj);
}
