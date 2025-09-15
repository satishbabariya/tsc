
// String iteration testing
function testStringIteration() {
    const text = "Hello, TypeScript!";
    const characters: string[] = [];
    
    // Iterate over string characters
    for (const char of text) {
        characters.push(char);
    }
    
    console.log("Characters:", characters);
    
    // Count vowels
    const vowels = "aeiouAEIOU";
    let vowelCount = 0;
    
    for (const char of text) {
        if (vowels.includes(char)) {
            vowelCount++;
        }
    }
    
    console.log("Vowel count:", vowelCount);
    
    // Count consonants
    let consonantCount = 0;
    for (const char of text) {
        if (/[a-zA-Z]/.test(char) && !vowels.includes(char)) {
            consonantCount++;
        }
    }
    
    console.log("Consonant count:", consonantCount);
}

// Test with multiple strings
function testMultipleStrings() {
    const words = ["hello", "world", "typescript", "programming"];
    const wordLengths: number[] = [];
    
    for (const word of words) {
        wordLengths.push(word.length);
    }
    
    console.log("Word lengths:", wordLengths);
    
    // Find longest word
    let longestWord = "";
    let maxLength = 0;
    
    for (const word of words) {
        if (word.length > maxLength) {
            maxLength = word.length;
            longestWord = word;
        }
    }
    
    console.log("Longest word:", longestWord, "Length:", maxLength);
}

// Test with string manipulation
function testStringManipulation() {
    const sentences = [
        "Hello world!",
        "TypeScript is awesome!",
        "Programming is fun!",
        "Code quality matters!"
    ];
    
    const wordCounts: number[] = [];
    
    for (const sentence of sentences) {
        const words = sentence.split(" ");
        wordCounts.push(words.length);
    }
    
    console.log("Word counts:", wordCounts);
    
    // Count total words
    let totalWords = 0;
    for (const sentence of sentences) {
        const words = sentence.split(" ");
        totalWords += words.length;
    }
    
    console.log("Total words:", totalWords);
}

// Run tests
testStringIteration();
testMultipleStrings();
testStringManipulation();
