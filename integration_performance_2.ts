
// Memory stress test
function memoryStress(): void {
    let arrays = []
    for (let i = 0; i < 1000; i++) {
        let arr = new Array(1000)
        arrays.push(arr)
    }
    // Access undefined property
    console.log(arrays.undefinedProperty)
}
