import { add } from "./math";
import { Button } from "./components/Button";
import { helpers } from "../utils/helpers"; // Should fail (directory traversal)

const result = add(1, 2);

export { add } from "./math"; // Re-export