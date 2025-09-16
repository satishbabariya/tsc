// Integration Tests for Array Operations
// These tests verify array operations work correctly within the larger compiler system

// Integration Test 1: Array Operations with Console Output
class ArrayConsoleIntegrationTest {
    items: number[];

    constructor() {
        this.items = [];
        _print("ArrayConsoleIntegrationTest constructor called");
    }

    addItem(item: number): void {
        this.items.push(item);
        _print("Added item:", item);
    }

    getLength(): number {
        let length = this.items.length;
        _print("Current array length:", length);
        return length;
    }

    processItems(): void {
        _print("Processing items...");
        let count = 0;
        // Simulate processing by adding items
        this.items.push(count++);
        this.items.push(count++);
        _print("Processing complete. Final length:", this.items.length);
    }
}

// Integration Test 2: Array Operations with Multiple Classes
class DataProcessor {
    items: number[];

    constructor() {
        this.items = [];
    }

    addData(item: number): void {
        this.items.push(item);
    }

    getDataCount(): number {
        return this.items.length;
    }
}

class DataValidator {
    processor: DataProcessor;

    constructor(processor: DataProcessor) {
        this.processor = processor;
    }

    validateData(): boolean {
        let count = this.processor.getDataCount();
        return count >= 0;
    }

    addValidatedData(item: number): void {
        if (item > 0) {
            this.processor.addData(item);
        }
    }
}

class DataManager {
    processor: DataProcessor;
    validator: DataValidator;

    constructor() {
        this.processor = new DataProcessor();
        this.validator = new DataValidator(this.processor);
    }

    processValidData(item: number): void {
        this.validator.addValidatedData(item);
    }

    getTotalDataCount(): number {
        return this.processor.getDataCount();
    }
}

// Integration Test 3: Array Operations with Inheritance
class BaseArrayHandler {
    items: number[];

    constructor() {
        this.items = [];
    }

    addItem(item: number): void {
        this.items.push(item);
    }

    getLength(): number {
        return this.items.length;
    }
}

class ExtendedArrayHandler extends BaseArrayHandler {
    addMultiple(items: number[]): void {
        for (let i = 0; i < items.length; i++) {
            this.addItem(items[i]);
        }
    }

    getExtendedLength(): number {
        return this.getLength();
    }
}

// Integration Test 4: Array Operations with Generic Constraints
class GenericDataContainer<T extends number> {
    items: T[];

    constructor() {
        this.items = [];
    }

    addItem(item: T): void {
        this.items.push(item);
    }

    getItemCount(): number {
        return this.items.length;
    }
}

class NumberDataContainer extends GenericDataContainer<number> {
    addPositiveOnly(item: number): void {
        if (item > 0) {
            this.addItem(item);
        }
    }
}

// Integration Test 5: Array Operations with Interface Implementation
interface ArrayManager {
    addItem(item: number): void;

    getItemCount(): number;

    isEmpty(): boolean;
}

class ArrayManagerImpl implements ArrayManager {
    items: number[];

    constructor() {
        this.items = [];
    }

    addItem(item: number): void {
        this.items.push(item);
    }

    getItemCount(): number {
        return this.items.length;
    }

    isEmpty(): boolean {
        return this.items.length === 0;
    }
}

// Integration Test 6: Array Operations with Complex Workflow
class WorkflowArrayManager {
    inputItems: number[];
    processedItems: number[];
    outputItems: number[];

    constructor() {
        this.inputItems = [];
        this.processedItems = [];
        this.outputItems = [];
    }

    addInput(item: number): void {
        this.inputItems.push(item);
    }

    processInputs(): void {
        for (let i = 0; i < this.inputItems.length; i++) {
            let processed = this.inputItems[i] * 2;
            this.processedItems.push(processed);
        }
    }

    generateOutputs(): void {
        for (let i = 0; i < this.processedItems.length; i++) {
            let output = this.processedItems[i] + 1;
            this.outputItems.push(output);
        }
    }

    getInputCount(): number {
        return this.inputItems.length;
    }

    getProcessedCount(): number {
        return this.processedItems.length;
    }

    getOutputCount(): number {
        return this.outputItems.length;
    }

    isWorkflowComplete(): boolean {
        return this.inputItems.length === this.processedItems.length &&
            this.processedItems.length === this.outputItems.length;
    }
}

// Integration Test 7: Array Operations with Error Handling Simulation
class ErrorHandlingArrayTest {
    items: number[];

    constructor() {
        this.items = [];
    }

    addItem(item: number): boolean {
        if (item >= 0) {
            this.items.push(item);
            return true;
        }
        return false;
    }

    getItemCount(): number {
        return this.items.length;
    }

    hasValidItems(): boolean {
        return this.items.length > 0;
    }
}

// Integration Test 8: Array Operations with Performance Simulation
class PerformanceArrayTest {
    items: number[];

    constructor() {
        this.items = [];
    }

    addBulkItems(count: number): void {
        for (let i = 0; i < count; i++) {
            this.items.push(i);
        }
    }

    getItemCount(): number {
        return this.items.length;
    }

    clearItems(): void {
        this.items = [];
    }
}

// Execute Integration Tests
_print("=== Starting Integration Tests ===");

// Test 1: Array Console Integration
let consoleTest = new ArrayConsoleIntegrationTest();
consoleTest.addItem(42);
consoleTest.getLength();
consoleTest.processItems();

// Test 2: Multiple Classes Integration
let dataManager = new DataManager();
dataManager.processValidData(10);
dataManager.processValidData(20);
_print("Data manager total count:", dataManager.getTotalDataCount());

// Test 3: Inheritance Integration
let extendedHandler = new ExtendedArrayHandler();
extendedHandler.addMultiple([1, 2, 3]);
_print("Extended handler length:", extendedHandler.getExtendedLength());

// Test 4: Generic Constraints Integration
let numberContainer = new NumberDataContainer();
numberContainer.addPositiveOnly(5);
numberContainer.addPositiveOnly(-1); // Should not be added
_print("Number container count:", numberContainer.getItemCount());

// Test 5: Interface Implementation Integration
let arrayManager = new ArrayManagerImpl();
arrayManager.addItem(100);
arrayManager.addItem(200);
_print("Array manager count:", arrayManager.getItemCount());
_print("Array manager is empty:", arrayManager.isEmpty());

// Test 6: Complex Workflow Integration
let workflowManager = new WorkflowArrayManager();
workflowManager.addInput(1);
workflowManager.addInput(2);
workflowManager.processInputs();
workflowManager.generateOutputs();
_print("Workflow input count:", workflowManager.getInputCount());
_print("Workflow processed count:", workflowManager.getProcessedCount());
_print("Workflow output count:", workflowManager.getOutputCount());
_print("Workflow complete:", workflowManager.isWorkflowComplete());

// Test 7: Error Handling Integration
let errorTest = new ErrorHandlingArrayTest();
let success1 = errorTest.addItem(5);
let success2 = errorTest.addItem(-1);
_print("Error test - add 5 success:", success1);
_print("Error test - add -1 success:", success2);
_print("Error test item count:", errorTest.getItemCount());
_print("Error test has valid items:", errorTest.hasValidItems());

// Test 8: Performance Integration
let performanceTest = new PerformanceArrayTest();
performanceTest.addBulkItems(5);
_print("Performance test item count:", performanceTest.getItemCount());
performanceTest.clearItems();
_print("Performance test after clear:", performanceTest.getItemCount());

_print("=== All Integration Tests Completed ===");