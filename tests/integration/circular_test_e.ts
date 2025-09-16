import {funcC} from "./circular_test_c";

export function funcE(): string {
    return "E" + funcC();
}