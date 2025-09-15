// Error case tests - these should produce compilation errors
// This file is for testing error handling and diagnostic messages

let invalidUnion: string |;  // Missing type after |
let anotherInvalid: | number;  // Missing type before |
let malformedUnion: string | number |;  // Trailing |
let emptyUnion: |;  // Empty union

// Invalid literal syntax
let badLiteral: "unclosed;  // Missing closing quote
let anotherBad: 'wrong quotes' | "mixed quotes";  // Mixed quote types

// Invalid array syntax
let badArray: string[] | number[] |;  // Trailing |
let malformedArray: (string | number |)[];  // Trailing | in parentheses

// Function with invalid union parameters
function badFunction(param: string |): void {  // Missing type
    // This should error
}

// Class with invalid union properties
class BadClass {
    badProp: string | number |;  // Trailing |
    anotherBad: | boolean;  // Missing type before |
}