// Circular Dependency Performance Test
// Test circular imports and complex dependency chains

// Module A
export interface ModuleA {
    processB: (b: ModuleB) => ModuleC;
    data: string;
}

export class ModuleAImpl implements ModuleA {
    data = "Module A";

    constructor(private moduleB: ModuleB) {
    }

    processB(b: ModuleB): ModuleC {
        return this.moduleB.processC(b);
    }
}

// Module B
export interface ModuleB {
    processC: (c: ModuleC) => ModuleA;
    data: number;
}

export class ModuleBImpl implements ModuleB {
    data = 42;

    constructor(private moduleC: ModuleC) {
    }

    processC(c: ModuleC): ModuleA {
        return this.moduleC.processA(c);
    }
}

// Module C
export interface ModuleC {
    processA: (a: ModuleA) => ModuleB;
    data: boolean;
}

export class ModuleCImpl implements ModuleC {
    data = true;

    constructor(private moduleA: ModuleA) {
    }

    processA(a: ModuleA): ModuleB {
        return this.moduleA.processB(a);
    }
}

// Circular dependency resolver
class CircularDependencyResolver {
    private modules = new Map<string, any>();

    register<T>(name: string, factory: () => T): void {
        this.modules.set(name, factory);
    }

    resolve<T>(name: string): T {
        const factory = this.modules.get(name);
        if (!factory) {
            throw new Error(`Module ${name} not found`);
        }
        return factory();
    }
}

// Performance test
function circularDependencyTest() {
    const startTime = performance.now();

    const resolver = new CircularDependencyResolver();

    // Register circular dependencies
    resolver.register('moduleA', () => new ModuleAImpl(resolver.resolve('moduleB')));
    resolver.register('moduleB', () => new ModuleBImpl(resolver.resolve('moduleC')));
    resolver.register('moduleC', () => new ModuleCImpl(resolver.resolve('moduleA')));

    // Test circular resolution
    for (let i = 0; i < 1000; i++) {
        const moduleA = resolver.resolve<ModuleA>('moduleA');
        const moduleB = resolver.resolve<ModuleB>('moduleB');
        const moduleC = resolver.resolve<ModuleC>('moduleC');

        // Test circular processing
        const resultA = moduleA.processB(moduleB);
        const resultB = moduleB.processC(moduleC);
        const resultC = moduleC.processA(moduleA);
    }

    const endTime = performance.now();
    console.log(`Circular dependency test completed in ${endTime - startTime}ms`);
}

circularDependencyTest();
console.log("Circular dependency performance test completed");
