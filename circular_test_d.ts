import { funcE } from "./circular_test_e";

export function funcD(): string {
    return "D" + funcE();
}