
// Complex class hierarchies for semantic coverage
class BaseClass {
    protected baseValue: number;
    
    constructor(value: number) {
        this.baseValue = value;
    }
    
    protected baseMethod(): number {
        return this.baseValue;
    }
}

class IntermediateClass extends BaseClass {
    protected intermediateValue: string;
    
    constructor(value: number, intermediate: string) {
        super(value);
        this.intermediateValue = intermediate;
    }
    
    protected intermediateMethod(): string {
        return this.intermediateValue;
    }
}

class FinalClass extends IntermediateClass {
    private finalValue: boolean;
    
    constructor(value: number, intermediate: string, final: boolean) {
        super(value, intermediate);
        this.finalValue = final;
    }
    
    public finalMethod(): string {
        return `${this.baseMethod()}:${this.intermediateMethod()}:${this.finalValue}`;
    }
}

function complexClassHierarchies(): void {
    let instance = new FinalClass(42, "test", true);
    console.log(instance.finalMethod());
}
