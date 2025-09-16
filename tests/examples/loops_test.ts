function testWhile() {
    let i = 0;
    while (i < 5) {
        let x = i * 2;
        i = i + 1;
    }
}

function testDoWhile() {
    let j = 0;
    do {
        let y = j + 1;
        j = j + 1;
    } while (j < 3);
}

function testFor() {
    for (let k = 0; k < 10; k = k + 1) {
        let z = k * k;
    }
}

function testForEmpty() {
    let count = 0;
    for (; ;) {
        count = count + 1;
        if (count > 100) {
            return;
        }
    }
}