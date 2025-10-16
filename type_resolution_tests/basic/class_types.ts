// Basic class type resolution tests
class Point {
    x: number;
    y: number;
    
    constructor(x: number, y: number) {
        this.x = x;
        this.y = y;
    }
    
    distance(): number {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }
    
    add(other: Point): Point {
        return new Point(this.x + other.x, this.y + other.y);
    }
}

// Test: class instantiation and method calls
let p1: Point = new Point(3, 4);
let p2: Point = new Point(1, 2);
let p3: Point = p1.add(p2);

let dist: number = p1.distance();
let sumDist: number = p3.distance();

console.log("Point 1 distance:", dist);
console.log("Point 3 distance:", sumDist);