import {funcX} from "./multi_circular_x";

export function funcY(): string {
    return "Y" + funcX();
}