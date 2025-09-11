// Template Literals
// Demonstrates template literal strings and string interpolation

// Basic template literals
let name: string = "Alice";
let age: number = 30;
let city: string = "New York";

// Simple template literal
let greeting: string = `Hello, ${name}!`;
let info: string = `${name} is ${age} years old and lives in ${city}`;

// Multi-line template literals
let multiLine: string = `
    User Information:
    Name: ${name}
    Age: ${age}
    City: ${city}
    Status: Active
`;

// Template literals with expressions
let calculation: string = `The sum of 5 and 3 is ${5 + 3}`;
let comparison: string = `${name} is ${age >= 18 ? "an adult" : "a minor"}`;

// Template literals with function calls
function formatCurrency(amount: number): string {
    return `$${amount.toFixed(2)}`;
}

let price: number = 99.99;
let priceString: string = `The price is ${formatCurrency(price)}`;

// Template literals with object properties
let person: object = {
    firstName: "John",
    lastName: "Doe",
    email: "john.doe@example.com"
};

let fullName: string = `${person.firstName} ${person.lastName}`;
let contactInfo: string = `Contact: ${person.email}`;

// Template literals with array elements
let fruits: string[] = ["apple", "banana", "cherry"];
let fruitList: string = `Available fruits: ${fruits.join(", ")}`;

// Template literals with conditional logic
let isLoggedIn: boolean = true;
let userStatus: string = `User is ${isLoggedIn ? "logged in" : "not logged in"}`;

// Template literals with nested expressions
let items: number[] = [1, 2, 3, 4, 5];
let sum: number = items.reduce((a, b) => a + b, 0);
let average: number = sum / items.length;
let stats: string = `Items: ${items.length}, Sum: ${sum}, Average: ${average.toFixed(2)}`;

// Template literals with escape sequences
let escaped: string = `This is a backtick: \` and this is a dollar sign: \${notInterpolated}`;

// Template literals with raw strings (concept)
let rawString: string = `Raw string with \n newline and \t tab`;

// Template literals for HTML generation
function createHTML(name: string, age: number): string {
    return `
        <div class="user-card">
            <h2>${name}</h2>
            <p>Age: ${age}</p>
            <p>Status: ${age >= 18 ? "Adult" : "Minor"}</p>
        </div>
    `;
}

let html: string = createHTML("Alice", 25);

// Template literals for SQL queries (concept)
function buildQuery(table: string, conditions: object): string {
    let whereClause: string = "";
    for (let key in conditions) {
        if (whereClause) {
            whereClause += " AND ";
        }
        whereClause += `${key} = '${conditions[key]}'`;
    }
    
    return `SELECT * FROM ${table} WHERE ${whereClause}`;
}

let query: string = buildQuery("users", { status: "active", age: 25 });

// Template literals for configuration
let config: object = {
    host: "localhost",
    port: 3000,
    database: "myapp"
};

let connectionString: string = `mongodb://${config.host}:${config.port}/${config.database}`;

// Template literals with date formatting
let now: Date = new Date();
let timestamp: string = `Generated at: ${now.getFullYear()}-${(now.getMonth() + 1).toString().padStart(2, '0')}-${now.getDate().toString().padStart(2, '0')}`;

// Template literals with error messages
function createErrorMessage(operation: string, error: string): string {
    return `Error in ${operation}: ${error}. Please try again.`;
}

let errorMsg: string = createErrorMessage("file upload", "permission denied");

// Template literals with logging
function logInfo(level: string, message: string, data: any): string {
    return `[${level.toUpperCase()}] ${new Date().toISOString()}: ${message} - ${JSON.stringify(data)}`;
}

let logEntry: string = logInfo("info", "User action", { userId: 123, action: "login" });

// Template literals with internationalization (concept)
let language: string = "en";
let messages: object = {
    en: {
        welcome: "Welcome",
        goodbye: "Goodbye"
    },
    es: {
        welcome: "Bienvenido",
        goodbye: "Adiós"
    }
};

let welcomeMessage: string = `${messages[language].welcome}, ${name}!`;

// Template literals with complex calculations
let measurements: object = {
    length: 10.5,
    width: 8.2,
    height: 3.1
};

let volume: number = measurements.length * measurements.width * measurements.height;
let surfaceArea: number = 2 * (measurements.length * measurements.width + 
                              measurements.length * measurements.height + 
                              measurements.width * measurements.height);

let geometryInfo: string = `
    Box Dimensions:
    Length: ${measurements.length} units
    Width: ${measurements.width} units
    Height: ${measurements.height} units
    Volume: ${volume.toFixed(2)} cubic units
    Surface Area: ${surfaceArea.toFixed(2)} square units
`;

// Template literals with nested template literals
let outerTemplate: string = `Outer: ${`Inner: ${name} is ${age}`} years old`;

// Template literals with function parameters
function createReport(title: string, data: any[], summary: string): string {
    let dataRows: string = data.map(item => `  - ${item}`).join("\n");
    
    return `
        Report: ${title}
        =================
        
        Data:
        ${dataRows}
        
        Summary: ${summary}
        
        Generated: ${new Date().toLocaleString()}
    `;
}

let reportData: string[] = ["Item 1", "Item 2", "Item 3"];
let report: string = createReport("Monthly Sales", reportData, "Sales increased by 15%");

// This example demonstrates comprehensive template literal functionality