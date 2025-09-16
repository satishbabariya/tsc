import {funcB} from "./circular_test_b";

export function funcA(): string {
    return "A" + funcB();
}