// Performance benchmark test for union types
// This file contains many union types to test compilation performance

let a1: string | number;
let a2: boolean | null;
let a3: "loading" | "success" | "error";
let a4: string | number | boolean;
let a5: number[] | string[];
let a6: User[] | null;

class User {
    id: string | number;
    status: "active" | "inactive";
    score: number | null;
}

let b1: string | number;
let b2: boolean | null;
let b3: "loading" | "success" | "error";
let b4: string | number | boolean;
let b5: number[] | string[];
let b6: User[] | null;

let c1: string | number;
let c2: boolean | null;
let c3: "loading" | "success" | "error";
let c4: string | number | boolean;
let c5: number[] | string[];
let c6: User[] | null;

let d1: string | number;
let d2: boolean | null;
let d3: "loading" | "success" | "error";
let d4: string | number | boolean;
let d5: number[] | string[];
let d6: User[] | null;

let e1: string | number;
let e2: boolean | null;
let e3: "loading" | "success" | "error";
let e4: string | number | boolean;
let e5: number[] | string[];
let e6: User[] | null;