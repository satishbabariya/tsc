/**
 * @fileoverview Standard Library - Array Implementation
 * 
 * This file contains the complete Array implementation for the TSC compiler.
 * It provides actual implementations for all array methods and properties.
 * 
 * The compiler will use this for:
 * - Type checking array method calls
 * - Resolving property access (e.g., array.length)
 * - Code generation for array operations
 * - Runtime implementation of array methods
 */

/**
 * Array implementation class
 * This is the actual implementation that will be used by the compiler
 */
class Array<T> {
    private data: T[];
    private length: number;

    constructor(initialCapacity: number = 10) {
        this.data = new Array<T>(initialCapacity);
        this.length = 0;
    }

    /**
     * Constructor from array literal
     */
    constructor(elements: T[]) {
        this.data = [...elements];
        this.length = elements.length;
    }

    /**
     * Gets the length of the array
     */
    get length(): number {
        return this.length;
    }

    /**
     * Sets the length of the array
     */
    set length(value: number) {
        if (value < 0) {
            throw new Error("Invalid array length");
        }
        
        if (value > this.length) {
            // Expand array
            this.data.length = value;
            for (let i = this.length; i < value; i++) {
                this.data[i] = undefined as T;
            }
        } else if (value < this.length) {
            // Shrink array
            this.data.length = value;
        }
        
        this.length = value;
    }

    /**
     * Appends new elements to an array, and returns the new length of the array.
     */
    push(...items: T[]): number {
        for (const item of items) {
            this.data[this.length] = item;
            this.length++;
        }
        return this.length;
    }

    /**
     * Removes the last element from an array and returns it.
     */
    pop(): T | undefined {
        if (this.length === 0) {
            return undefined;
        }
        
        this.length--;
        const item = this.data[this.length];
        this.data[this.length] = undefined as T;
        return item;
    }

    /**
     * Removes the first element from an array and returns it.
     */
    shift(): T | undefined {
        if (this.length === 0) {
            return undefined;
        }
        
        const item = this.data[0];
        
        // Shift all elements left
        for (let i = 0; i < this.length - 1; i++) {
            this.data[i] = this.data[i + 1];
        }
        
        this.length--;
        this.data[this.length] = undefined as T;
        return item;
    }

    /**
     * Adds new elements to the beginning of an array, and returns the new length of the array.
     */
    unshift(...items: T[]): number {
        // Shift existing elements right
        for (let i = this.length - 1; i >= 0; i--) {
            this.data[i + items.length] = this.data[i];
        }
        
        // Insert new items at the beginning
        for (let i = 0; i < items.length; i++) {
            this.data[i] = items[i];
        }
        
        this.length += items.length;
        return this.length;
    }

    /**
     * Combines two or more arrays.
     */
    concat(...items: T[]): T[] {
        const result = new Array<T>();
        result.data = [...this.data.slice(0, this.length)];
        result.length = this.length;
        
        for (const item of items) {
            if (Array.isArray(item)) {
                for (const element of item) {
                    result.push(element);
                }
            } else {
                result.push(item);
            }
        }
        
        return result.data.slice(0, result.length);
    }

    /**
     * Returns a section of an array.
     */
    slice(start?: number, end?: number): T[] {
        const startIdx = start ?? 0;
        const endIdx = end ?? this.length;
        
        if (startIdx < 0) {
            startIdx = this.length + startIdx;
        }
        if (endIdx < 0) {
            endIdx = this.length + endIdx;
        }
        
        const result: T[] = [];
        for (let i = startIdx; i < endIdx && i < this.length; i++) {
            result.push(this.data[i]);
        }
        
        return result;
    }

    /**
     * Removes elements from an array and, if necessary, inserts new elements in their place.
     */
    splice(start: number, deleteCount?: number, ...items: T[]): T[] {
        const startIdx = start < 0 ? Math.max(0, this.length + start) : start;
        const delCount = deleteCount ?? (this.length - startIdx);
        
        const removed: T[] = [];
        
        // Collect removed elements
        for (let i = startIdx; i < startIdx + delCount && i < this.length; i++) {
            removed.push(this.data[i]);
        }
        
        // Shift elements left to fill gaps
        const shiftAmount = delCount - items.length;
        if (shiftAmount > 0) {
            for (let i = startIdx + delCount; i < this.length; i++) {
                this.data[i - shiftAmount] = this.data[i];
            }
            this.length -= shiftAmount;
        } else if (shiftAmount < 0) {
            // Shift elements right to make room
            for (let i = this.length - 1; i >= startIdx + delCount; i--) {
                this.data[i - shiftAmount] = this.data[i];
            }
            this.length -= shiftAmount;
        }
        
        // Insert new items
        for (let i = 0; i < items.length; i++) {
            this.data[startIdx + i] = items[i];
        }
        
        return removed;
    }

    /**
     * Returns the index of the first occurrence of a value in an array, or -1 if it is not present.
     */
    indexOf(searchElement: T, fromIndex?: number): number {
        const startIdx = fromIndex ?? 0;
        
        for (let i = startIdx; i < this.length; i++) {
            if (this.data[i] === searchElement) {
                return i;
            }
        }
        
        return -1;
    }

    /**
     * Returns the index of the last occurrence of a value in an array, or -1 if it is not present.
     */
    lastIndexOf(searchElement: T, fromIndex?: number): number {
        const startIdx = fromIndex ?? this.length - 1;
        
        for (let i = startIdx; i >= 0; i--) {
            if (this.data[i] === searchElement) {
                return i;
            }
        }
        
        return -1;
    }

    /**
     * Performs the specified action for each element in an array.
     */
    forEach(callbackfn: (value: T, index: number, array: T[]) => void): void {
        for (let i = 0; i < this.length; i++) {
            callbackfn(this.data[i], i, this.data.slice(0, this.length));
        }
    }

    /**
     * Calls a defined callback function on each element of an array, and returns an array that contains the results.
     */
    map<U>(callbackfn: (value: T, index: number, array: T[]) => U): U[] {
        const result: U[] = [];
        for (let i = 0; i < this.length; i++) {
            result.push(callbackfn(this.data[i], i, this.data.slice(0, this.length)));
        }
        return result;
    }

    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     */
    filter(callbackfn: (value: T, index: number, array: T[]) => boolean): T[] {
        const result: T[] = [];
        for (let i = 0; i < this.length; i++) {
            if (callbackfn(this.data[i], i, this.data.slice(0, this.length))) {
                result.push(this.data[i]);
            }
        }
        return result;
    }

    /**
     * Calls the specified callback function for all the elements in an array.
     */
    reduce<U>(callbackfn: (previousValue: U, currentValue: T, currentIndex: number, array: T[]) => U, initialValue: U): U {
        let result = initialValue;
        for (let i = 0; i < this.length; i++) {
            result = callbackfn(result, this.data[i], i, this.data.slice(0, this.length));
        }
        return result;
    }

    /**
     * Calls the specified callback function for all the elements in an array, in descending order.
     */
    reduceRight<U>(callbackfn: (previousValue: U, currentValue: T, currentIndex: number, array: T[]) => U, initialValue: U): U {
        let result = initialValue;
        for (let i = this.length - 1; i >= 0; i--) {
            result = callbackfn(result, this.data[i], i, this.data.slice(0, this.length));
        }
        return result;
    }

    /**
     * Determines whether all the members of an array satisfy the specified test.
     */
    every(callbackfn: (value: T, index: number, array: T[]) => boolean): boolean {
        for (let i = 0; i < this.length; i++) {
            if (!callbackfn(this.data[i], i, this.data.slice(0, this.length))) {
                return false;
            }
        }
        return true;
    }

    /**
     * Determines whether the specified callback function returns true for any element of an array.
     */
    some(callbackfn: (value: T, index: number, array: T[]) => boolean): boolean {
        for (let i = 0; i < this.length; i++) {
            if (callbackfn(this.data[i], i, this.data.slice(0, this.length))) {
                return true;
            }
        }
        return false;
    }

    /**
     * Returns the value of the first element in the array where predicate is true, and undefined otherwise.
     */
    find(predicate: (value: T, index: number, array: T[]) => boolean): T | undefined {
        for (let i = 0; i < this.length; i++) {
            if (predicate(this.data[i], i, this.data.slice(0, this.length))) {
                return this.data[i];
            }
        }
        return undefined;
    }

    /**
     * Returns the index of the first element in the array where predicate is true, and -1 otherwise.
     */
    findIndex(predicate: (value: T, index: number, array: T[]) => boolean): number {
        for (let i = 0; i < this.length; i++) {
            if (predicate(this.data[i], i, this.data.slice(0, this.length))) {
                return i;
            }
        }
        return -1;
    }

    /**
     * Returns a new array with all sub-array elements concatenated into it recursively up to the specified depth.
     */
    flat<U>(depth: number = 1): U[] {
        const result: U[] = [];
        
        for (let i = 0; i < this.length; i++) {
            const item = this.data[i];
            if (Array.isArray(item) && depth > 0) {
                const flattened = (item as any).flat(depth - 1);
                for (const element of flattened) {
                    result.push(element);
                }
            } else {
                result.push(item as U);
            }
        }
        
        return result;
    }

    /**
     * Calls a defined callback function on each element of an array. Then, flattens the result into a new array.
     */
    flatMap<U>(callbackfn: (value: T, index: number, array: T[]) => U | U[]): U[] {
        const result: U[] = [];
        
        for (let i = 0; i < this.length; i++) {
            const mapped = callbackfn(this.data[i], i, this.data.slice(0, this.length));
            if (Array.isArray(mapped)) {
                for (const element of mapped) {
                    result.push(element);
                }
            } else {
                result.push(mapped);
            }
        }
        
        return result;
    }

    /**
     * Returns a new array with all elements sorted.
     */
    sort(compareFn?: (a: T, b: T) => number): T[] {
        const result = this.data.slice(0, this.length);
        
        if (compareFn) {
            result.sort(compareFn);
        } else {
            result.sort();
        }
        
        return result;
    }

    /**
     * Reverses the elements in an array.
     */
    reverse(): T[] {
        const result: T[] = [];
        for (let i = this.length - 1; i >= 0; i--) {
            result.push(this.data[i]);
        }
        return result;
    }

    /**
     * Fills all the elements of an array from a start index to an end index with a static value.
     */
    fill(value: T, start?: number, end?: number): T[] {
        const startIdx = start ?? 0;
        const endIdx = end ?? this.length;
        
        for (let i = startIdx; i < endIdx && i < this.length; i++) {
            this.data[i] = value;
        }
        
        return this.data.slice(0, this.length);
    }

    /**
     * Returns an array of key, value pairs for every entry in the array
     */
    entries(): Array<[number, T]> {
        const result: Array<[number, T]> = [];
        for (let i = 0; i < this.length; i++) {
            result.push([i, this.data[i]]);
        }
        return result;
    }

    /**
     * Returns an array of keys in the array
     */
    keys(): Array<number> {
        const result: Array<number> = [];
        for (let i = 0; i < this.length; i++) {
            result.push(i);
        }
        return result;
    }

    /**
     * Returns an array of values in the array
     */
    values(): Array<T> {
        return this.data.slice(0, this.length);
    }

    /**
     * Returns a string representation of an array.
     */
    toString(): string {
        if (this.length === 0) {
            return "";
        }
        
        let result = "";
        for (let i = 0; i < this.length; i++) {
            if (i > 0) {
                result += ",";
            }
            result += String(this.data[i]);
        }
        return result;
    }

    /**
     * Returns a string representation of an array. The elements are converted to string using their toLocaleString methods.
     */
    toLocaleString(): string {
        if (this.length === 0) {
            return "";
        }
        
        let result = "";
        for (let i = 0; i < this.length; i++) {
            if (i > 0) {
                result += ",";
            }
            result += (this.data[i] as any).toLocaleString?.() ?? String(this.data[i]);
        }
        return result;
    }

    /**
     * Index access operator
     */
    [n: number]: T {
        get {
            if (n >= 0 && n < this.length) {
                return this.data[n];
            }
            return undefined as T;
        }
        set {
            if (n >= 0 && n < this.length) {
                this.data[n] = value;
            }
        }
    }
}

/**
 * Array constructor implementation
 */
class ArrayConstructor {
    /**
     * Creates a new array with the specified length
     */
    constructor(arrayLength?: number) {
        return new Array(arrayLength ?? 0);
    }

    /**
     * Creates a new array with the specified elements
     */
    constructor<T>(...items: T[]): T[] {
        return new Array(items);
    }

    /**
     * Returns true if the value is an array
     */
    static isArray(arg: any): arg is Array<any> {
        return arg instanceof Array;
    }

    /**
     * The Array prototype object
     */
    static prototype: Array<any> = new Array();
}

/**
 * Global Array constructor
 */
declare var Array: ArrayConstructor;