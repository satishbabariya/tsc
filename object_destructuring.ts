
// Object destructuring testing
interface Person {
    name: string;
    age: number;
    city: string;
    country: string;
}

function testObjectDestructuring() {
    const person: Person = {
        name: "Alice",
        age: 30,
        city: "New York",
        country: "USA"
    };
    
    // Basic object destructuring
    const { name, age } = person;
    console.log("Name:", name);
    console.log("Age:", age);
    
    // Object destructuring with renaming
    const { name: personName, age: personAge } = person;
    console.log("Person name:", personName);
    console.log("Person age:", personAge);
    
    // Object destructuring with default values
    const { name: userName = "Unknown", age: userAge = 0, email = "no-email" } = person;
    console.log("User name:", userName);
    console.log("User age:", userAge);
    console.log("Email:", email);
    
    // Object destructuring with rest
    const { name: fullName, ...otherProps } = person;
    console.log("Full name:", fullName);
    console.log("Other props:", otherProps);
}

// Test with nested objects
function testNestedObjectDestructuring() {
    const user = {
        id: 1,
        profile: {
            name: "Bob",
            age: 25,
            address: {
                street: "123 Main St",
                city: "Boston",
                state: "MA"
            }
        },
        settings: {
            theme: "dark",
            notifications: true
        }
    };
    
    const { 
        id, 
        profile: { 
            name, 
            address: { city, state } 
        },
        settings: { theme }
    } = user;
    
    console.log("ID:", id);
    console.log("Name:", name);
    console.log("City:", city);
    console.log("State:", state);
    console.log("Theme:", theme);
}

// Test with function parameters
function testObjectDestructuringParameters() {
    function processUser({ name, age, city }: Person) {
        console.log("Processing user:", name, age, city);
    }
    
    function processUserWithDefaults({ name = "Unknown", age = 0, city = "Unknown" }: Partial<Person>) {
        console.log("Processing user with defaults:", name, age, city);
    }
    
    const user: Person = {
        name: "Charlie",
        age: 35,
        city: "San Francisco",
        country: "USA"
    };
    
    processUser(user);
    processUserWithDefaults({});
}

// Run tests
testObjectDestructuring();
testNestedObjectDestructuring();
testObjectDestructuringParameters();
