
// Integration Regression Test
// Test integration between different features

// Module system with generics
export interface GenericModule<T> {
    data: T;
    process: (value: T) => T;
    export: () => T;
}

export class GenericModuleImpl<T> implements GenericModule<T> {
    constructor(public data: T) {}
    
    process(value: T): T {
        return value;
    }
    
    export(): T {
        return this.data;
    }
}

// Namespace with generics
namespace GenericNamespace {
    export interface GenericInterface<T> {
        value: T;
        method: () => T;
    }
    
    export class GenericClass<T> implements GenericInterface<T> {
        constructor(public value: T) {}
        
        method(): T {
            return this.value;
        }
    }
    
    export function genericFunction<T>(value: T): T {
        return value;
    }
}

// Union types with modules
type ModuleUnion = GenericModule<string> | GenericModule<number>;
type NamespaceUnion = GenericNamespace.GenericInterface<string> | GenericNamespace.GenericInterface<number>;

// Intersection types with modules
type ModuleIntersection = GenericModule<string> & { metadata: string };
type NamespaceIntersection = GenericNamespace.GenericInterface<string> & { extra: string };

// Conditional types with modules
type ModuleConditional<T> = T extends string ? GenericModule<T> : GenericNamespace.GenericInterface<T>;
type NamespaceConditional<T> = T extends number ? GenericNamespace.GenericClass<T> : GenericModule<T>;

// Mapped types with modules
type ModuleMapped<T> = {
    [K in keyof T]: GenericModule<T[K]>;
};
type NamespaceMapped<T> = {
    [K in keyof T]: GenericNamespace.GenericInterface<T[K]>;
};

// Integration test
function testIntegration() {
    // Test module with generics
    const stringModule = new GenericModuleImpl<string>('hello');
    const numberModule = new GenericModuleImpl<number>(42);
    
    console.log('String module:', stringModule.export());
    console.log('Number module:', numberModule.export());
    
    // Test namespace with generics
    const stringClass = new GenericNamespace.GenericClass<string>('world');
    const numberClass = new GenericNamespace.GenericClass<number>(84);
    
    console.log('String class:', stringClass.method());
    console.log('Number class:', numberClass.method());
    
    // Test generic function
    const stringResult = GenericNamespace.genericFunction<string>('test');
    const numberResult = GenericNamespace.genericFunction<number>(123);
    
    console.log('String function result:', stringResult);
    console.log('Number function result:', numberResult);
    
    // Test unions
    const moduleUnion: ModuleUnion = stringModule;
    const namespaceUnion: NamespaceUnion = stringClass;
    
    console.log('Module union:', moduleUnion);
    console.log('Namespace union:', namespaceUnion);
    
    // Test intersections
    const moduleIntersection: ModuleIntersection = {
        ...stringModule,
        metadata: 'test metadata'
    };
    const namespaceIntersection: NamespaceIntersection = {
        ...stringClass,
        extra: 'extra data'
    };
    
    console.log('Module intersection:', moduleIntersection);
    console.log('Namespace intersection:', namespaceIntersection);
    
    // Test conditionals
    const moduleConditional: ModuleConditional<string> = stringModule;
    const namespaceConditional: NamespaceConditional<number> = numberClass;
    
    console.log('Module conditional:', moduleConditional);
    console.log('Namespace conditional:', namespaceConditional);
    
    // Test mapped types
    const moduleMapped: ModuleMapped<{ name: string; age: number }> = {
        name: stringModule,
        age: numberModule
    };
    const namespaceMapped: NamespaceMapped<{ title: string; count: number }> = {
        title: stringClass,
        count: numberClass
    };
    
    console.log('Module mapped:', moduleMapped);
    console.log('Namespace mapped:', namespaceMapped);
}

testIntegration();
console.log("Integration regression test completed");
