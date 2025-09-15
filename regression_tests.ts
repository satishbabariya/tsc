
// Regression tests to ensure existing functionality remains intact
interface LegacyInterface {
    id: number;
    name: string;
}

// Test that existing non-generic code still works
function legacyFunction(item: LegacyInterface): string {
    return `${item.id}: ${item.name}`;
}

// Test that generic code doesn't break existing functionality
class LegacyClass {
    private items: LegacyInterface[] = [];
    
    add(item: LegacyInterface): void {
        this.items.push(item);
    }
    
    getById(id: number): LegacyInterface | undefined {
        return this.items.find(item => item.id === id);
    }
    
    getAll(): LegacyInterface[] {
        return this.items;
    }
}

// Test generic code alongside legacy code
class GenericLegacyClass<T extends LegacyInterface> {
    private items: T[] = [];
    
    add(item: T): void {
        this.items.push(item);
    }
    
    getById(id: number): T | undefined {
        return this.items.find(item => item.id === id);
    }
    
    getAll(): T[] {
        return this.items;
    }
    
    // Generic method that works with legacy interface
    processLegacy(item: LegacyInterface): string {
        return legacyFunction(item);
    }
}

// Test that generic constraints work with existing interfaces
interface ExistingInterface {
    id: number;
    value: string;
}

function processExisting<T extends ExistingInterface>(item: T): T {
    return item;
}

// Test that generic arrays work with existing arrays
function processExistingArray(items: LegacyInterface[]): LegacyInterface[] {
    return items.map(item => ({ ...item, name: item.name.toUpperCase() }));
}

// Test that generic functions work with existing functions
function combineLegacyAndGeneric<T extends LegacyInterface>(
    legacyItem: LegacyInterface,
    genericItem: T
): { legacy: string; generic: T } {
    return {
        legacy: legacyFunction(legacyItem),
        generic: genericItem
    };
}

// Test that generic classes work with existing classes
class ExtendedLegacyClass<T extends LegacyInterface> extends LegacyClass {
    private genericItems: T[] = [];
    
    addGeneric(item: T): void {
        this.genericItems.push(item);
    }
    
    getGenericById(id: number): T | undefined {
        return this.genericItems.find(item => item.id === id);
    }
    
    getAllGeneric(): T[] {
        return this.genericItems;
    }
    
    // Method that combines legacy and generic functionality
    processBoth(legacyId: number, genericId: number): { legacy?: LegacyInterface; generic?: T } {
        return {
            legacy: this.getById(legacyId),
            generic: this.getGenericById(genericId)
        };
    }
}

// Test that generic methods work with existing methods
class MixedClass {
    private legacyItems: LegacyInterface[] = [];
    private genericItems: any[] = [];
    
    // Legacy method
    addLegacy(item: LegacyInterface): void {
        this.legacyItems.push(item);
    }
    
    // Generic method
    addGeneric<T>(item: T): void {
        this.genericItems.push(item);
    }
    
    // Method that uses both
    processAll(): { legacy: LegacyInterface[]; generic: any[] } {
        return {
            legacy: this.legacyItems,
            generic: this.genericItems
        };
    }
}

// Test that generic constraints work with existing constraints
interface ExistingConstraint {
    compare(other: ExistingConstraint): number;
}

function sortExisting<T extends ExistingConstraint>(items: T[]): T[] {
    return items.sort((a, b) => a.compare(b));
}

// Test that generic type inference works with existing types
function inferFromExisting<T>(items: T[]): T[] {
    return items.filter((item, index) => index % 2 === 0);
}

// Test that generic arrays work with existing array methods
function processWithExistingMethods<T extends LegacyInterface>(items: T[]): T[] {
    return items
        .filter(item => item.id > 0)
        .map(item => ({ ...item, name: item.name.toUpperCase() }))
        .sort((a, b) => a.id - b.id);
}

// Run regression tests
const legacyItem: LegacyInterface = { id: 1, name: 'Legacy Item' };
const existingItem: ExistingInterface = { id: 2, value: 'Existing Value' };

console.log('Legacy function:', legacyFunction(legacyItem));
console.log('Process existing:', processExisting(existingItem));
console.log('Combine legacy and generic:', combineLegacyAndGeneric(legacyItem, legacyItem));

const legacyClass = new LegacyClass();
legacyClass.add(legacyItem);
console.log('Legacy class:', legacyClass.getAll());

const genericLegacyClass = new GenericLegacyClass<LegacyInterface>();
genericLegacyClass.add(legacyItem);
console.log('Generic legacy class:', genericLegacyClass.getAll());
console.log('Process legacy:', genericLegacyClass.processLegacy(legacyItem));

const extendedClass = new ExtendedLegacyClass<LegacyInterface>();
extendedClass.add(legacyItem);
extendedClass.addGeneric(legacyItem);
console.log('Extended class both:', extendedClass.processBoth(1, 1));

const mixedClass = new MixedClass();
mixedClass.addLegacy(legacyItem);
mixedClass.addGeneric(existingItem);
console.log('Mixed class all:', mixedClass.processAll());

console.log('Regression tests completed successfully');
