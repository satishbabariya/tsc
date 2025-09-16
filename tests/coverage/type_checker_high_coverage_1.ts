// Comprehensive type checking for high coverage
interface BaseInterface {
    id: number;
    name: string;
}

interface ExtendedInterface extends BaseInterface {
    value: number;

    method(): string;
}

class BaseClass implements BaseInterface {
    id: number;
    name: string;

    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
    }
}

class ExtendedClass extends BaseClass implements ExtendedInterface {
    value: number;

    constructor(id: number, name: string, value: number) {
        super(id, name);
        this.value = value;
    }

    method(): string {
        return `${this.name}:${this.value}`;
    }
}

function comprehensiveTypeChecking(): void {
    // Type annotations
    let num: number = 42;
    let str: string = "hello";
    let bool: boolean = true;
    let arr: number[] = [1, 2, 3];
    let obj: { name: string; value: number } = {name: "test", value: 42};

    // Generic types
    function genericFunction<T>(value: T): T {
        return value;
    }

    let genericResult = genericFunction<number>(42);
    let genericString = genericFunction<string>("hello");

    // Union types
    let union: string | number = "hello";
    union = 42;

    // Intersection types
    let intersection: BaseInterface & { value: number } = {
        id: 1,
        name: "test",
        value: 42
    };

    // Type assertions
    let anyValue: any = "hello";
    let assertedString = anyValue as string;

    // Class instantiation and inheritance
    let baseInstance = new BaseClass(1, "base");
    let extendedInstance = new ExtendedClass(2, "extended", 42);

    // Method calls
    console.log(baseInstance.name);
    console.log(extendedInstance.method());

    // Array operations with types
    let typedArray: ExtendedClass[] = [extendedInstance];
    typedArray.push(new ExtendedClass(3, "another", 100));

    console.log(num, str, bool, arr, obj, genericResult, genericString, union, intersection, assertedString, typedArray);
}
