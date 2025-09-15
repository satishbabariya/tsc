
// Error-heavy code compilation benchmark
// This tests compilation performance with extensive error handling

class ErrorHeavyService {
    async processWithExtensiveErrorHandling(data: any[]): Promise<Result<any[], BaseError>> {
        const results: any[] = [];
        
        for (let i = 0; i < data.length; i++) {
            try {
                const item = data[i];
                
                // Multiple validation layers
                if (!item) {
                    throw new ValidationError("Item is null or undefined", "item", item);
                }
                
                if (typeof item !== "object") {
                    throw new ValidationError("Item must be an object", "item", item);
                }
                
                if (!item.id) {
                    throw new ValidationError("Item ID is required", "id", item.id);
                }
                
                if (!item.name) {
                    throw new ValidationError("Item name is required", "name", item.name);
                }
                
                // Simulate complex processing
                await new Promise(resolve => setTimeout(resolve, 1));
                
                // Additional error conditions
                if (item.id === "error") {
                    throw new DatabaseError("Database error for item", "SELECT * FROM items WHERE id = ?", "items");
                }
                
                if (item.name.includes("network")) {
                    throw new NetworkError("Network error for item", "https://api.example.com/items/" + item.id, 500);
                }
                
                results.push({
                    ...item,
                    processed: true,
                    timestamp: new Date().toISOString()
                });
                
            } catch (error: BaseError) {
                // Handle specific error types
                if (error instanceof ValidationError) {
                    console.log("Validation error:", error.message);
                    continue; // Skip invalid items
                } else if (error instanceof DatabaseError) {
                    console.log("Database error:", error.message);
                    return Result.err(error);
                } else if (error instanceof NetworkError) {
                    console.log("Network error:", error.message);
                    return Result.err(error);
                } else {
                    console.log("Unknown error:", error.message);
                    return Result.err(error);
                }
            } catch (error: Error) {
                return Result.err(new ValidationError(`Unexpected error: ${error.message}`, "processWithExtensiveErrorHandling", item));
            }
        }
        
        return Result.ok(results);
    }
    
    async processWithNestedErrorHandling(data: any[]): Promise<Result<any[], BaseError>> {
        try {
            const results: any[] = [];
            
            for (const item of data) {
                try {
                    const processedItem = await this.processItemWithNestedErrors(item);
                    if (processedItem.isOk()) {
                        results.push(processedItem.unwrap());
                    } else {
                        // Handle nested error
                        const error = processedItem.unwrapOr({} as any);
                        console.log("Nested processing error:", error);
                    }
                } catch (error: Error) {
                    console.log("Item processing error:", error.message);
                }
            }
            
            return Result.ok(results);
        } catch (error: Error) {
            return Result.err(new ValidationError(`Unexpected error: ${error.message}`, "processWithNestedErrorHandling", data));
        }
    }
    
    private async processItemWithNestedErrors(item: any): Promise<Result<any, BaseError>> {
        try {
            // First level validation
            if (!item) {
                throw new ValidationError("Item is null", "item", item);
            }
            
            // Second level validation
            if (!item.id) {
                throw new ValidationError("Item ID is required", "id", item.id);
            }
            
            // Third level validation
            if (typeof item.id !== "string") {
                throw new ValidationError("Item ID must be a string", "id", item.id);
            }
            
            // Simulate processing
            await new Promise(resolve => setTimeout(resolve, 1));
            
            return Result.ok({
                ...item,
                processed: true,
                nested: true
            });
        } catch (error: BaseError) {
            return Result.err(error);
        } catch (error: Error) {
            return Result.err(new ValidationError(`Unexpected error: ${error.message}`, "processItemWithNestedErrors", item));
        }
    }
}

// Test error-heavy compilation
console.log("=== Error-Heavy Code Compilation Benchmark ===");

const errorHeavyService = new ErrorHeavyService();

// Test with extensive error handling
const testData = [
    { id: "1", name: "Item 1" },
    { id: "2", name: "Item 2" },
    { id: "error", name: "Error Item" },
    { id: "3", name: "network test" },
    { id: "4", name: "Item 4" },
    null,
    { id: "5" }, // Missing name
    { id: 123, name: "Invalid ID" }, // Invalid ID type
    { id: "6", name: "Item 6" }
];

errorHeavyService.processWithExtensiveErrorHandling(testData).then(result => {
    if (result.isOk()) {
        console.log("Extensive error handling successful:", result.unwrap().length, "items processed");
    } else {
        console.log("Extensive error handling failed:", result.unwrapOr([]));
    }
});

errorHeavyService.processWithNestedErrorHandling(testData).then(result => {
    if (result.isOk()) {
        console.log("Nested error handling successful:", result.unwrap().length, "items processed");
    } else {
        console.log("Nested error handling failed:", result.unwrapOr([]));
    }
});
