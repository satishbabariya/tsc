
// Complex function with nested control statements
function complexNestedFunction(x: number, y: string, z: boolean): string {
    let result = "";
    
    // Level 1: if-else
    if (x > 0) {
        // Level 2: switch
        switch (y.length) {
            case 1:
                // Level 3: for loop
                for (let i = 0; i < x; i++) {
                    // Level 4: while loop
                    while (i < 10) {
                        // Level 5: if-else
                        if (z) {
                            // Level 6: do-while
                            do {
                                // Level 7: nested switch
                                switch (i % 3) {
                                    case 0:
                                        // Level 8: for loop
                                        for (let j = 0; j < 5; j++) {
                                            // Level 9: if-else
                                            if (j % 2 === 0) {
                                                // Level 10: try-catch
                                                try {
                                                    result += `Level10: ${i}-${j}`;
                                                } catch (error) {
                                                    result += "Error";
                                                }
                                            } else {
                                                result += `Odd: ${j}`;
                                            }
                                        }
                                        break;
                                    case 1:
                                        result += "One";
                                        break;
                                    case 2:
                                        result += "Two";
                                        break;
                                }
                                i++;
                            } while (i < 5);
                        } else {
                            result += "False";
                        }
                    }
                }
                break;
            case 2:
                // Level 2: while loop
                while (x > 0) {
                    // Level 3: for loop
                    for (let k = 0; k < y.length; k++) {
                        // Level 4: if-else
                        if (k % 2 === 0) {
                            // Level 5: switch
                            switch (k) {
                                case 0:
                                    result += "Zero";
                                    break;
                                case 2:
                                    result += "Two";
                                    break;
                                default:
                                    result += "Default";
                            }
                        }
                    }
                    x--;
                }
                break;
            default:
                // Level 2: do-while
                do {
                    // Level 3: for loop
                    for (let m = 0; m < 3; m++) {
                        // Level 4: while loop
                        while (m < 2) {
                            // Level 5: if-else
                            if (m === 0) {
                                result += "Start";
                            } else {
                                result += "Middle";
                            }
                            m++;
                        }
                    }
                    x++;
                } while (x < 10);
        }
    } else {
        // Level 1: else - switch
        switch (y) {
            case "negative":
                // Level 2: for loop
                for (let n = 0; n < Math.abs(x); n++) {
                    // Level 3: while loop
                    while (n < 5) {
                        // Level 4: if-else
                        if (n % 2 === 0) {
                            result += "Even";
                        } else {
                            result += "Odd";
                        }
                        n++;
                    }
                }
                break;
            default:
                // Level 2: while loop
                while (x < 0) {
                    // Level 3: for loop
                    for (let p = 0; p < 3; p++) {
                        result += "Negative";
                    }
                    x++;
                }
        }
    }
    
    return result;
}
