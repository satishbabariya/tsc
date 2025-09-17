// Type Error Test Case
function typeErrorTest(): void {
    let num: number = 42;
    let str: string = "hello";
    let bool: boolean = true;

    // Type mismatches
    let result1 = num + str;
    let result2 = str + bool;
    let result3 = num + bool;

    // Assignment type mismatches
    let numberVar: number = "string";
    let stringVar: string = 42;
    let booleanVar: boolean = "true";

    // Function parameter type mismatches
    function processNumber(n: number): number {
        return n * 2;
    }

    let result4 = processNumber(str);
    let result5 = processNumber(bool);

    // Array type mismatches
    let numberArray: number[] = ["1", "2", "3"];
    let stringArray: string[] = [1, 2, 3];
}
