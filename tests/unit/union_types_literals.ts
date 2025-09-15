// String literal union type tests
let status: "loading" | "success" | "error";
let mode: "active" | "inactive";
let direction: "up" | "down" | "left" | "right";

// Boolean literal union type tests
let flag: true | false;
let state: true | null;

// Mixed literal and primitive unions
let mixed: "hello" | number | boolean;
let complex: "loading" | "success" | number | null;

// Function with literal union parameters
function setStatus(status: "loading" | "success" | "error"): void {
    // Set status
}

function toggleFlag(flag: true | false): boolean {
    return !flag;
}

// Class with literal union properties
class StateMachine {
    currentState: "idle" | "running" | "paused" | "stopped";
    isEnabled: true | false;
    priority: "low" | "medium" | "high" | null;
}