function testSwitch() {
    let x = 2;
    switch (x) {
        case 1:
            let a = 10;
            break;
        case 2:
            let b = 20;
            break;
        case 3:
            let c = 30;
            break;
        default:
            let d = 40;
            break;
    }
}

function testSwitchNoBreak() {
    let y = 1;
    switch (y) {
        case 1:
            let e = 100;
        case 2:
            let f = 200;
        default:
            let g = 300;
    }
}

function testSwitchMinimal() {
    switch (1) {
        case 1:
            let h = 42;
    }
}