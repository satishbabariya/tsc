// Calculator module that imports from math_utils
import { add, subtract, multiply, PI } from "./math_utils";

export class Calculator {
    private result: number = 0;
    
    add(value: number): Calculator {
        this.result = add(this.result, value);
        return this;
    }
    
    subtract(value: number): Calculator {
        this.result = subtract(this.result, value);
        return this;
    }
    
    multiply(value: number): Calculator {
        this.result = multiply(this.result, value);
        return this;
    }
    
    getResult(): number {
        return this.result;
    }
    
    reset(): Calculator {
        this.result = 0;
        return this;
    }
}

export function createCalculator(): Calculator {
    return new Calculator();
}