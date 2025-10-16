// Simplified class type tests (without Math or complex features)
class Point {
    x: number;
    y: number;
    
    constructor(x: number, y: number) {
        this.x = x;
        this.y = y;
    }
    
    add(other: Point): Point {
        return new Point(this.x + other.x, this.y + other.y);
    }
    
    getX(): number {
        return this.x;
    }
    
    getY(): number {
        return this.y;
    }
}

// Test: class instantiation and method calls
let p1: Point = new Point(3, 4);
let p2: Point = new Point(1, 2);
let p3: Point = p1.add(p2);

let x1: number = p1.getX();
let y1: number = p1.getY();
let x3: number = p3.getX();
let y3: number = p3.getY();

console.log("Point 1:", x1, y1);
console.log("Point 3:", x3, y3);