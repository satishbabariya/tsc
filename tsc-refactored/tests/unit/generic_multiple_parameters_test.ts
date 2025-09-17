// Test multiple type parameters with array methods

// Test with two type parameters
class TwoParamContainer<T, U> {
    private primary: T[];
    private secondary: U[];

    addPrimary(item: T): void {
        this.primary.push(item);
    }

    addSecondary(item: U): void {
        this.secondary.push(item);
    }

    getPrimaryLength(): number {
        return this.primary.length;
    }

    getSecondaryLength(): number {
        return this.secondary.length;
    }

    testPrimaryPush(item: T): number {
        return this.primary.push(item);
    }

    testSecondaryPush(item: U): number {
        return this.secondary.push(item);
    }
}

// Test with three type parameters
class ThreeParamContainer<T, U, V> {
    private first: T[];
    private second: U[];
    private third: V[];

    addFirst(item: T): void {
        this.first.push(item);
    }

    addSecond(item: U): void {
        this.second.push(item);
    }

    addThird(item: V): void {
        this.third.push(item);
    }

    getFirstLength(): number {
        return this.first.length;
    }

    getSecondLength(): number {
        return this.second.length;
    }

    getThirdLength(): number {
        return this.third.length;
    }
}

// Test with mixed type parameters
class MixedParamContainer<T, U> {
    private data: T[];
    private metadata: U[];
    private combined: T[];

    addData(item: T): void {
        this.data.push(item);
        this.combined.push(item);
    }

    addMetadata(item: U): void {
        this.metadata.push(item);
    }

    getDataLength(): number {
        return this.data.length;
    }

    getMetadataLength(): number {
        return this.metadata.length;
    }

    getCombinedLength(): number {
        return this.combined.length;
    }
}