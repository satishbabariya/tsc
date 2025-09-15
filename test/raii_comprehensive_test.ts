// Comprehensive RAII Test - Demonstrating all phases working together
// Phase 1: Destructor Syntax ✅
// Phase 2: Semantic Analysis ✅  
// Phase 3: Code Generation ✅
// Phase 4: Runtime Integration ✅

class ResourceManager {
    private resource: string;
    private isAcquired: boolean = false;
    
    constructor(resource: string) {
        this.resource = resource;
        this.isAcquired = true;
        console.log(`Resource acquired: ${this.resource}`);
    }
    
    // Phase 1: Destructor syntax parsing
    ~ResourceManager() {
        if (this.isAcquired) {
            console.log(`Resource released: ${this.resource}`);
            this.isAcquired = false;
        }
    }
    
    getResource(): string {
        return this.resource;
    }
}

class FileHandler {
    private filename: string;
    private isOpen: boolean = false;
    
    constructor(filename: string) {
        this.filename = filename;
        this.isOpen = true;
        console.log(`File opened: ${this.filename}`);
    }
    
    // Phase 1: Destructor syntax parsing
    ~FileHandler() {
        if (this.isOpen) {
            console.log(`File closed: ${this.filename}`);
            this.isOpen = false;
        }
    }
    
    getFilename(): string {
        return this.filename;
    }
}

class NetworkConnection {
    private host: string;
    private port: number;
    private isConnected: boolean = false;
    
    constructor(host: string, port: number) {
        this.host = host;
        this.port = port;
        this.isConnected = true;
        console.log(`Connected to ${this.host}:${this.port}`);
    }
    
    // Phase 1: Destructor syntax parsing
    ~NetworkConnection() {
        if (this.isConnected) {
            console.log(`Disconnected from ${this.host}:${this.port}`);
            this.isConnected = false;
        }
    }
    
    getHost(): string {
        return this.host;
    }
    
    getPort(): number {
        return this.port;
    }
}

// Test function to demonstrate RAII
function testRAII() {
    console.log("=== RAII Comprehensive Test ===");
    
    // Create resources - destructors will be called automatically
    let resource1 = new ResourceManager("Database Connection");
    let file1 = new FileHandler("config.txt");
    let network1 = new NetworkConnection("localhost", 8080);
    
    // Use the resources
    console.log(`Using resource: ${resource1.getResource()}`);
    console.log(`Using file: ${file1.getFilename()}`);
    console.log(`Using network: ${network1.getHost()}:${network1.getPort()}`);
    
    // Resources will be automatically cleaned up when they go out of scope
    console.log("Resources will be cleaned up automatically via RAII");
}

// Run the test
testRAII();