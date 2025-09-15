
// Double free simulation
function doubleFree(): void {
    let obj = { data: "test" }
    // Simulate double free
    obj = null
    obj = null
}
