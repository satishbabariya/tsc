// Test nested generic types with array methods

// Test nested arrays
class NestedArrayContainer<T> {
    private matrix: T[][];
    private flat: T[];

    addToMatrix(row: T[]): void {
        this.matrix.push(row);
    }

    addToFlat(item: T): void {
        this.flat.push(item);
    }

    getMatrixLength(): number {
        return this.matrix.length;
    }

    getFlatLength(): number {
        return this.flat.length;
    }

    testMatrixPush(row: T[]): number {
        return this.matrix.push(row);
    }

    testFlatPush(item: T): number {
        return this.flat.push(item);
    }
}

// Test generic containers within generic containers
class ContainerOfContainers<T> {
    private containers: T[];
    private backup: T[];

    addContainer(container: T): void {
        this.containers.push(container);
    }

    backupContainers(): void {
        this.backup = this.containers;
    }

    getContainerCount(): number {
        return this.containers.length;
    }

    getBackupCount(): number {
        return this.backup.length;
    }

    testContainerPush(container: T): number {
        return this.containers.push(container);
    }
}

// Test generic interfaces with nested types
interface NestedInterface<T> {
    data: T[];
    metadata: T[];
}

class NestedInterfaceContainer<T> {
    private nested: NestedInterface<T>;

    addToData(item: T): void {
        this.nested.data.push(item);
    }

    addToMetadata(item: T): void {
        this.nested.metadata.push(item);
    }

    getDataLength(): number {
        return this.nested.data.length;
    }

    getMetadataLength(): number {
        return this.nested.metadata.length;
    }

    testDataPush(item: T): number {
        return this.nested.data.push(item);
    }

    testMetadataPush(item: T): number {
        return this.nested.metadata.push(item);
    }
}