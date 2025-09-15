function testCatchNoParam() {
    try {
        throw "error";
    } catch {
        _print("Caught something");
    }
}