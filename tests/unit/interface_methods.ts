// Interface with methods
interface Logger {
    log(message: string): void;

    error(message: string): void;

    warn(message: string): void;

    info(message: string): void;
}

interface Calculator {
    add(a: number, b: number): number;

    subtract(a: number, b: number): number;

    multiply(a: number, b: number): number;

    divide(a: number, b: number): number;
}

interface DataProcessor {
    process(data: string): string;

    validate(input: any): boolean;

    getResult(): string | number;

    reset(): void;
}