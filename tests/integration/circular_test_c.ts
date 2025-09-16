import {funcD} from "./circular_test_d";

export function funcC(): string {
    return "C" + funcD();
}