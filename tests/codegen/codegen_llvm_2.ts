// Large array allocation
function largeArray(): void {
    let arr = new Array(1000000)
    for (let i = 0; i < 1000000; i++) {
        arr[i] = i * i
    }
}
