import {funcA} from "./circular_test_a";

export function funcB(): string {
    return "B" + funcA();
}