// Complex nested structures for parser coverage
function complexNestedStructures(): void {
    // Deeply nested if statements
    if (true) {
        if (true) {
            if (true) {
                if (true) {
                    if (true) {
                        console.log("deep nested");
                    }
                }
            }
        }
    }

    // Deeply nested loops
    for (let i = 0; i < 3; i++) {
        for (let j = 0; j < 3; j++) {
            for (let k = 0; k < 3; k++) {
                for (let l = 0; l < 3; l++) {
                    for (let m = 0; m < 3; m++) {
                        console.log(i, j, k, l, m);
                    }
                }
            }
        }
    }

    // Complex switch statements
    switch (1) {
        case 1:
            switch (2) {
                case 2:
                    switch (3) {
                        case 3:
                            console.log("triple nested switch");
                            break;
                    }
                    break;
            }
            break;
    }
}
