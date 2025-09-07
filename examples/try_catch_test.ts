function testTryCatch() {
    try {
        let x = 10;
        throw "Something went wrong";
        return x;
    } catch (e) {
        console.log("Caught error: " + e);
        return -1;
    } finally {
        console.log("Cleanup complete");
    }
}