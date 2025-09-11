function testCatchNoParam() {
    try {
        throw "error";
    } catch {
        console.log("Caught something");
    }
}