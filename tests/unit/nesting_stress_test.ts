// Deep Nesting Stress Test
function nestingStressTest(): void {
    let level1 = 1;
    if (level1 > 0) {
        let level2 = 2;
        if (level2 > 1) {
            let level3 = 3;
            if (level3 > 2) {
                let level4 = 4;
                if (level4 > 3) {
                    let level5 = 5;
                    if (level5 > 4) {
                        let level6 = 6;
                        if (level6 > 5) {
                            let level7 = 7;
                            if (level7 > 6) {
                                let level8 = 8;
                                if (level8 > 7) {
                                    let level9 = 9;
                                    if (level9 > 8) {
                                        let level10 = 10;
                                        console.log("Deep nesting level 10");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Nested loops
    for (let i = 0; i < 10; i++) {
        for (let j = 0; j < 10; j++) {
            for (let k = 0; k < 10; k++) {
                for (let l = 0; l < 10; l++) {
                    for (let m = 0; m < 10; m++) {
                        console.log(i, j, k, l, m);
                    }
                }
            }
        }
    }
}
