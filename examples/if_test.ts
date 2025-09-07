function testIf() {
    let x = 5;
    if (x > 0) {
        return 1;
    }
    return 0;
}

function testIfElse() {
    let y = -3;
    if (y > 0) {
        return 1;
    } else {
        return -1;
    }
}

function testNestedIf() {
    let z = 10;
    if (z > 0) {
        if (z > 5) {
            return 2;
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}