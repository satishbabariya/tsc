// Test simpler edge cases
function testEdgeCases() {
    // Empty function
    function empty() {}
    
    // Complex nested structures
    let complex = {
        data: [
            {id: 1, values: [10, 20]}
        ],
        config: {
            nested: {
                value: "test"
            }
        }
    };
    
    // Single assignments
    let a = 10;
    let b = 20;
    let c = 30;
    
    // Complex expressions with parentheses
    let result = (a + b) * c;
    
    // Nested control flow
    for (let i = 0; i < 2; i = i + 1) {
        if (i == 0) {
            switch (result) {
                case 42:
                    break;
                default:
                    continue;
            }
        }
    }
    
    return result;
}