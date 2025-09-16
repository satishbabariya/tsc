// Complex edge cases and boundary conditions
function advancedEdgeCases(): void {
    // Numeric edge cases
    let numericEdgeCases = {
        zero: 0,
        negativeZero: -0,
        infinity: Infinity,
        negativeInfinity: -Infinity,
        nan: NaN,
        maxSafeInteger: Number.MAX_SAFE_INTEGER,
        minSafeInteger: Number.MIN_SAFE_INTEGER,
        maxValue: Number.MAX_VALUE,
        minValue: Number.MIN_VALUE,
        epsilon: Number.EPSILON,
        veryLarge: 1e308,
        verySmall: 1e-308,
        precision: 0.1 + 0.2, // Floating point precision issue
        overflow: Number.MAX_VALUE * 2,
        underflow: Number.MIN_VALUE / 2
    };

    // String edge cases
    let stringEdgeCases = {
        empty: "",
        singleChar: "a",
        unicode: "🚀🌟💫⭐✨🔥💥⚡🌈🎉🎊🎈🎁🎀🎂🍰🎪🎭🎨🎬🎵🎶🎸🎹🎺🎻🎼🎽🎾🎿🏀🏁🏂🏃🏄🏅🏆🏇🏈🏉🏊🏋🏌🏍🏎🏏🏐🏑🏒🏓🏔🏕🏖🏗🏘🏙🏚🏛🏜🏝🏞🏟🏠🏡🏢🏣🏤🏥🏦🏧🏨🏩🏪🏫🏬🏭🏮🏯🏰🏱🏲🏳🏴🏵🏶🏷🏸🏹🏺🏻🏼🏽🏾🏿",
        specialChars: "!@#$%^&*()_+-=[]{}|;':\",./<>?`~",
        newlines: "line1\nline2\rline3\r\nline4",
        tabs: "col1\tcol2\tcol3",
        quotes: 'He said "Hello" and \'Goodbye\'',
        backslashes: "C:\\Users\\Name\\Documents",
        veryLong: "a".repeat(1000000),
        nullChars: "hello\0world\0test"
    };

    // Array edge cases
    let arrayEdgeCases = {
        empty: [],
        singleElement: [42],
        twoElements: [1, 2],
        largeArray: new Array(1000000).fill(0),
        sparseArray: [1, , , 4, , 6],
        mixedTypes: [1, "hello", true, null, undefined, {}, []],
        nestedArrays: [[1, 2], [3, 4], [5, 6]],
        deepNested: [[[[[[[[[[42]]]]]]]]]],
        circular: [] as any[]
    };

    // Create circular reference
    arrayEdgeCases.circular.push(arrayEdgeCases.circular);

    // Test edge cases
    console.log("Numeric edge cases:", numericEdgeCases);
    console.log("String edge cases:", stringEdgeCases);
    console.log("Array edge cases:", arrayEdgeCases);
}
