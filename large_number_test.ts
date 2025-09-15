
// Large Number Test
function largeNumberTest(): void {
    let hugeNumber = 9223372036854775807;
    let anotherHugeNumber = 18446744073709551615;
    let maxSafeInteger = 9007199254740991;
    let minSafeInteger = -9007199254740991;
    
    let infinity = Infinity;
    let negativeInfinity = -Infinity;
    let nan = NaN;
    
    let result1 = hugeNumber + anotherHugeNumber;
    let result2 = maxSafeInteger * 2;
    let result3 = minSafeInteger - 1;
    
    console.log("Huge number:", hugeNumber);
    console.log("Another huge number:", anotherHugeNumber);
    console.log("Max safe integer:", maxSafeInteger);
    console.log("Min safe integer:", minSafeInteger);
    console.log("Infinity:", infinity);
    console.log("Negative infinity:", negativeInfinity);
    console.log("NaN:", nan);
}
