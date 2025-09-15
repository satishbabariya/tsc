function test() {
    try {
        throw "error";
    } catch {
        let handled = true;
    }
}