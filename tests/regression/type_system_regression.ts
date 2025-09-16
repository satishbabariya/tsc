// Type system regression test
function testTypeInference(): void {
    const inferredString = "hello";
    const inferredNumber = 42;
    const inferredArray = [1, 2, 3];

    const explicitString: string = inferredString;
    const explicitNumber: number = inferredNumber;
    const explicitArray: number[] = inferredArray;

    console.log("Type inference test passed");
}

interface Lengthwise {
    length: number;
}

function loggingIdentity<T extends Lengthwise>(arg: T): T {
    console.log(arg.length);
    return arg;
}

function testTypeConstraints(): void {
    const stringResult = loggingIdentity("hello");
    const arrayResult = loggingIdentity([1, 2, 3]);
    console.log("Type constraints test passed");
}

type NonNullable<T> = T extends null | undefined ? never : T;

function testConditionalTypes(): void {
    type StringOrNull = string | null;
    type NonNullString = NonNullable<StringOrNull>;
    console.log("Conditional types test passed");
}

type Partial<T> = {
    [P in keyof T]?: T[P];
};

function testMappedTypes(): void {
    interface User {
        id: string;
        name: string;
        email: string;
        age?: number;
    }

    type PartialUser = Partial<User>;
    const partialUser: PartialUser = {name: "Alice"};
    console.log("Mapped types test passed");
}

type EventName<T extends string> = `on${Capitalize<T>}`;

function testTemplateLiteralTypes(): void {
    type ClickEvent = EventName<"click">;
    type ChangeEvent = EventName<"change">;
    console.log("Template literal types test passed");
}

function testUtilityTypes(): void {
    interface User {
        id: string;
        name: string;
        email: string;
        age: number;
        permissions: string[];
    }

    type UserKeys = keyof User;
    type UserValues = User[keyof User];
    type PickUser = Pick<User, "id" | "name">;
    type OmitUser = Omit<User, "age" | "permissions">;

    const pickUser: PickUser = {id: "1", name: "Alice"};
    const omitUser: OmitUser = {id: "1", name: "Alice", email: "alice@example.com"};

    console.log("Utility types test passed");
}

function isString(value: unknown): value is string {
    return typeof value === "string";
}

function testTypeGuards(): void {
    const unknownValue: unknown = "hello";

    if (isString(unknownValue)) {
        console.log(unknownValue.toUpperCase());
    }

    console.log("Type guards test passed");
}

function testTypeAssertions(): void {
    const someValue: unknown = "hello";
    const stringValue = someValue as string;
    const stringValue2 = <string>someValue;
    console.log("Type assertions test passed");
}

console.log("=== Type System Regression Test ===");
testTypeInference();
testTypeConstraints();
testConditionalTypes();
testMappedTypes();
testTemplateLiteralTypes();
testUtilityTypes();
testTypeGuards();
testTypeAssertions();
console.log("All type system tests passed!");
