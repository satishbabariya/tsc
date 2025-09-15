// Simple Destructor Test
class SimpleClass {
    private name: string;
    
    constructor(name: string) {
        this.name = name;
        console.log(`Created: ${name}`);
    }
    
    ~SimpleClass() {
        console.log(`Destroyed: ${this.name}`);
    }
    
    getName(): string {
        return this.name;
    }
}

function testDestructor(): void {
    console.log("Testing destructor syntax");
    let obj = new SimpleClass("test");
    console.log(`Object name: ${obj.getName()}`);
}

testDestructor();