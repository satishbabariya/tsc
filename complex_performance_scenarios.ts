
// Complex performance scenarios
function advancedPerformanceStress(): number {
    let result = 0;
    
    // Complex nested loops with heavy computations
    for (let i = 0; i < 1000; i++) {
        for (let j = 0; j < 1000; j++) {
            for (let k = 0; k < 1000; k++) {
                // Heavy mathematical computations
                let value = Math.sqrt(i * j * k) + 
                           Math.sin(i) * Math.cos(j) + 
                           Math.tan(k) * Math.log(i + 1) +
                           Math.pow(i, 2) + Math.pow(j, 2) + Math.pow(k, 2);
                
                // Complex conditional logic
                if (value > 0) {
                    if (i % 2 === 0) {
                        if (j % 3 === 0) {
                            if (k % 5 === 0) {
                                result += Math.sqrt(value) * Math.sin(value) * Math.cos(value);
                            } else {
                                result += Math.tan(value) * Math.log(value + 1);
                            }
                        } else {
                            result += Math.pow(value, 1.5) * Math.exp(value / 1000);
                        }
                    } else {
                        result += Math.abs(value) * Math.floor(value);
                    }
                } else {
                    result += Math.abs(value) * Math.ceil(value);
                }
            }
        }
    }
    
    return result;
}

// Recursive performance stress
function recursivePerformanceStress(n: number, depth: number = 0): number {
    if (depth > 1000 || n <= 0) {
        return 1;
    }
    
    let result = 0;
    
    // Heavy computation at each recursive level
    for (let i = 0; i < n; i++) {
        result += Math.sqrt(i) * Math.sin(i) * Math.cos(i) * Math.tan(i);
    }
    
    // Recursive calls
    let recursiveResult1 = recursivePerformanceStress(n - 1, depth + 1);
    let recursiveResult2 = recursivePerformanceStress(n - 2, depth + 1);
    let recursiveResult3 = recursivePerformanceStress(n - 3, depth + 1);
    
    return result + recursiveResult1 + recursiveResult2 + recursiveResult3;
}
