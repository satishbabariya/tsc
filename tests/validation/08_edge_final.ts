// Test final edge cases
function testEdgeCases() {
    // Empty function
    function empty() {
        // Empty body
    }

    // Complex nested object
    let complex = {
        data: [{id: 1}],
        config: {value: "test"}
    };

    // Expression with parentheses
    let a = 10;
    let b = 20;
    let result = (a + b) * 2;

    // Simple nested control flow
    if (result > 0) {
        for (let i = 0; i < 1; i = i + 1) {
            if (i == 0) {
                result = result + 1;
            }
        }
    }

    return result;
}