// Main module that uses both math_utils and calculator
import { PI } from "./math_utils";
import { createCalculator } from "./calculator";

export function main(): number {
    const calc = createCalculator();
    
    // Perform some calculations
    const result = calc
        .add(10)
        .multiply(2)
        .subtract(5)
        .getResult();
    
    // Use imported constant
    const circleArea = PI * 5 * 5;
    
    return result + circleArea;
}