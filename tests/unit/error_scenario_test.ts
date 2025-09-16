// Error Scenario Regression Test
// Test error handling and validation

// Invalid type scenarios
type InvalidUnion = string | number | string; // Duplicate string
type InvalidIntersection = { name: string } & { name: number }; // Conflicting properties

// Complex error scenarios
interface ErrorInterface<T> {
    data: T;
    process: (value: T) => T;
    validate: (value: T) => boolean;
}

class ErrorClass<T> implements ErrorInterface<T> {
    constructor(public data: T) {
    }

    process(value: T): T {
        if (typeof value !== typeof this.data) {
            throw new Error('Type mismatch');
        }
        return value;
    }

    validate(value: T): boolean {
        try {
            this.process(value);
            return true;
        } catch (error) {
            console.error('Validation error:', error);
            return false;
        }
    }
}

// Error handling test
function testErrorScenarios() {
    try {
        // Test type mismatch
        const errorClass = new ErrorClass<string>('hello');
        const isValid = errorClass.validate(42); // Should return false

        if (!isValid) {
            console.log('Type mismatch error handled correctly');
        }

        // Test process error
        try {
            errorClass.process(42); // Should throw error
        } catch (error) {
            console.log('Process error handled correctly:', error.message);
        }

        // Test valid scenario
        const validResult = errorClass.process('world');
        console.log('Valid process result:', validResult);

        const validValidation = errorClass.validate('world');
        console.log('Valid validation result:', validValidation);

    } catch (error) {
        console.error('Unexpected error:', error);
    }
}

testErrorScenarios();
console.log("Error scenario regression test completed");
