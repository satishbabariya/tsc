// Test edge cases and error handling
function testEdgeCases() {
    // Empty function
    function empty() {
    }

    // Complex nested structures
    let complex = {
        data: [
            {id: 1, values: [10, 20]},
            {id: 2, values: [30, 40]}
        ],
        config: {
            nested: {
                deep: {
                    value: "test"
                }
            }
        }
    };

    // Multiple assignments
    let a, b, c;
    a = b = c = 42;

    // Complex expressions
    let result = (a + b) * c - (10 / 2);

    // Nested control flow
    for (let i = 0; i < 2; i = i + 1) {
        if (i == 0) {
            switch (result) {
                case 42:
                    try {
                        throw "error";
                    } catch (e) {
                        break;
                    }
                default:
                    continue;
            }
        }
    }

    return result;
}