function test() {
    try {
        let x = 10;
        throw x;
    } catch (e) {
        let y = e;
        return y;
    } finally {
        let z = 1;
    }
}