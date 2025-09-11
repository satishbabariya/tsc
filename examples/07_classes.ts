// Classes
// Demonstrates class declarations, inheritance, methods, and properties

// Basic class declaration
class Person {
    // Properties
    name: string;
    age: number;
    isActive: boolean;

    // Constructor
    constructor(name: string, age: number) {
        this.name = name;
        this.age = age;
        this.isActive = true;
    }

    // Methods
    greet(): string {
        return "Hello, I'm " + this.name;
    }

    getAge(): number {
        return this.age;
    }

    setAge(newAge: number): void {
        if (newAge >= 0) {
            this.age = newAge;
        }
    }

    // Method with parameters
    introduceTo(other: Person): string {
        return this.name + " meets " + other.name;
    }
}

// Class with inheritance
class Student extends Person {
    // Additional properties
    studentId: string;
    grade: number;

    // Constructor with super call
    constructor(name: string, age: number, studentId: string) {
        super(name, age);
        this.studentId = studentId;
        this.grade = 0;
    }

    // Override method
    greet(): string {
        return "Hi, I'm " + this.name + " (Student ID: " + this.studentId + ")";
    }

    // Additional methods
    setGrade(grade: number): void {
        if (grade >= 0 && grade <= 100) {
            this.grade = grade;
        }
    }

    getGrade(): number {
        return this.grade;
    }

    // Method that calls parent method
    fullIntroduction(): string {
        return super.greet() + " and I'm a student";
    }
}

// Class with static members
class MathUtils {
    // Static property
    static PI: number = 3.14159;

    // Static method
    static add(a: number, b: number): number {
        return a + b;
    }

    static multiply(a: number, b: number): number {
        return a * b;
    }

    static circleArea(radius: number): number {
        return this.PI * radius * radius;
    }
}

// Class with private and public members
class BankAccount {
    private balance: number;
    public accountNumber: string;

    constructor(accountNumber: string, initialBalance: number = 0) {
        this.accountNumber = accountNumber;
        this.balance = initialBalance;
    }

    // Public methods
    public deposit(amount: number): boolean {
        if (amount > 0) {
            this.balance += amount;
            return true;
        }
        return false;
    }

    public withdraw(amount: number): boolean {
        if (amount > 0 && amount <= this.balance) {
            this.balance -= amount;
            return true;
        }
        return false;
    }

    public getBalance(): number {
        return this.balance;
    }

    // Private method
    private validateAmount(amount: number): boolean {
        return amount > 0;
    }
}

// Abstract class (concept)
class Shape {
    protected color: string;

    constructor(color: string) {
        this.color = color;
    }

    // Abstract method (concept - would be abstract in real TypeScript)
    getArea(): number {
        return 0; // Default implementation
    }

    getColor(): string {
        return this.color;
    }
}

// Concrete class extending abstract class
class Rectangle extends Shape {
    private width: number;
    private height: number;

    constructor(color: string, width: number, height: number) {
        super(color);
        this.width = width;
        this.height = height;
    }

    getArea(): number {
        return this.width * this.height;
    }

    getWidth(): number {
        return this.width;
    }

    getHeight(): number {
        return this.height;
    }
}

class Circle extends Shape {
    private radius: number;

    constructor(color: string, radius: number) {
        super(color);
        this.radius = radius;
    }

    getArea(): number {
        return MathUtils.PI * this.radius * this.radius;
    }

    getRadius(): number {
        return this.radius;
    }
}

// Class with getter and setter
class Temperature {
    private _celsius: number;

    constructor(celsius: number) {
        this._celsius = celsius;
    }

    get celsius(): number {
        return this._celsius;
    }

    set celsius(value: number) {
        this._celsius = value;
    }

    get fahrenheit(): number {
        return (this._celsius * 9 / 5) + 32;
    }

    set fahrenheit(value: number) {
        this._celsius = (value - 32) * 5 / 9;
    }
}

// Class with method overloading (using union types)
class Calculator {
    add(a: number, b: number): number;
    add(a: string, b: string): string;
    add(a: number | string, b: number | string): number | string {
        if (typeof a === "number" && typeof b === "number") {
            return a + b;
        } else {
            return a.toString() + b.toString();
        }
    }
}

// Usage examples
let person1: Person = new Person("Alice", 25);
let person2: Person = new Person("Bob", 30);

let greeting: string = person1.greet();
let age: number = person1.getAge();
person1.setAge(26);

let student1: Student = new Student("Charlie", 20, "STU001");
let studentGreeting: string = student1.greet();
let fullIntro: string = student1.fullIntroduction();
student1.setGrade(85);
let grade: number = student1.getGrade();

// Static member usage
let pi: number = MathUtils.PI;
let sum: number = MathUtils.add(5, 3);
let product: number = MathUtils.multiply(4, 7);
let area: number = MathUtils.circleArea(5);

// Bank account usage
let account: BankAccount = new BankAccount("ACC123", 1000);
let deposited: boolean = account.deposit(500);
let withdrawn: boolean = account.withdraw(200);
let balance: number = account.getBalance();

// Shape usage
let rectangle: Rectangle = new Rectangle("red", 10, 5);
let circle: Circle = new Circle("blue", 3);
let rectArea: number = rectangle.getArea();
let circleArea: number = circle.getArea();

// Temperature usage
let temp: Temperature = new Temperature(25);
let celsius: number = temp.celsius;
let fahrenheit: number = temp.fahrenheit;
temp.fahrenheit = 86;

// Calculator usage
let calc: Calculator = new Calculator();
let numResult: number = calc.add(5, 3);
let strResult: string = calc.add("Hello", "World");

// This example demonstrates comprehensive class functionality