// Test basic functionality without nested functions
function testBasic() {
    let a = 10;
    let b = 20;
    let result = (a + b) * 2;

    if (result > 0) {
        result = result + 1;
    }

    return result;
}