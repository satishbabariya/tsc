// Complex interface type resolution tests
interface Drawable {
    draw(): void;
    getArea(): number;
}

interface Movable {
    move(x: number, y: number): void;
    getPosition(): {x: number, y: number};
}

class Rectangle implements Drawable, Movable {
    x: number;
    y: number;
    width: number;
    height: number;
    
    constructor(x: number, y: number, width: number, height: number) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }
    
    draw(): void {
        console.log("Drawing rectangle");
    }
    
    getArea(): number {
        return this.width * this.height;
    }
    
    move(x: number, y: number): void {
        this.x += x;
        this.y += y;
    }
    
    getPosition(): {x: number, y: number} {
        return {x: this.x, y: this.y};
    }
}

// Test: interface type resolution
let rect: Drawable & Movable = new Rectangle(10, 20, 30, 40);
let area: number = rect.getArea();
rect.move(5, 10);
let pos: {x: number, y: number} = rect.getPosition();

console.log("Area:", area);
console.log("Position:", pos.x, pos.y);