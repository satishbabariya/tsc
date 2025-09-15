import { funcY } from "./multi_circular_y";
import { funcZ } from "./multi_circular_z";

export function funcX(): string {
    return "X" + funcY() + funcZ();
}