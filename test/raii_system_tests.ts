// RAII System Tests - Real-world usage scenarios
// Tests for complex object graphs, resource management patterns, and error conditions

// System Test 1: File System Resource Management
class FileHandle {
    private filename: string;
    private isOpen: boolean;
    private fileSize: number;
    
    constructor(filename: string) {
        this.filename = filename;
        this.isOpen = true;
        this.fileSize = 0;
        console.log(`File '${filename}' opened`);
    }
    
    ~FileHandle() {
        if (this.isOpen) {
            console.log(`Closing file '${this.filename}' (size: ${this.fileSize} bytes)`);
            this.isOpen = false;
        }
    }
    
    getFilename(): string {
        return this.filename;
    }
    
    getFileSize(): number {
        return this.fileSize;
    }
    
    setFileSize(size: number): void {
        this.fileSize = size;
    }
    
    isFileOpen(): boolean {
        return this.isOpen;
    }
}

// System Test 2: Database Connection Management
class DatabaseConnection {
    private connectionString: string;
    private isConnected: boolean;
    private transactionCount: number;
    
    constructor(connectionString: string) {
        this.connectionString = connectionString;
        this.isConnected = true;
        this.transactionCount = 0;
        console.log(`Database connection established: ${connectionString}`);
    }
    
    ~DatabaseConnection() {
        if (this.isConnected) {
            console.log(`Closing database connection: ${this.connectionString}`);
            console.log(`Final transaction count: ${this.transactionCount}`);
            this.isConnected = false;
        }
    }
    
    getConnectionString(): string {
        return this.connectionString;
    }
    
    isConnectionActive(): boolean {
        return this.isConnected;
    }
    
    beginTransaction(): void {
        if (this.isConnected) {
            this.transactionCount++;
            console.log(`Transaction ${this.transactionCount} begun`);
        }
    }
    
    commitTransaction(): void {
        if (this.isConnected && this.transactionCount > 0) {
            console.log(`Transaction ${this.transactionCount} committed`);
            this.transactionCount--;
        }
    }
}

// System Test 3: Network Socket Management
class NetworkSocket {
    private host: string;
    private port: number;
    private isConnected: boolean;
    private bytesTransferred: number;
    
    constructor(host: string, port: number) {
        this.host = host;
        this.port = port;
        this.isConnected = true;
        this.bytesTransferred = 0;
        console.log(`Socket connected to ${host}:${port}`);
    }
    
    ~NetworkSocket() {
        if (this.isConnected) {
            console.log(`Closing socket to ${this.host}:${this.port}`);
            console.log(`Total bytes transferred: ${this.bytesTransferred}`);
            this.isConnected = false;
        }
    }
    
    getHost(): string {
        return this.host;
    }
    
    getPort(): number {
        return this.port;
    }
    
    isSocketConnected(): boolean {
        return this.isConnected;
    }
    
    transferData(bytes: number): void {
        if (this.isConnected) {
            this.bytesTransferred += bytes;
            console.log(`Transferred ${bytes} bytes (total: ${this.bytesTransferred})`);
        }
    }
}

// System Test 4: Memory Pool Management
class MemoryPool {
    private poolSize: number;
    private allocatedBlocks: number;
    private freeBlocks: number;
    private name: string;
    
    constructor(name: string, poolSize: number) {
        this.name = name;
        this.poolSize = poolSize;
        this.allocatedBlocks = 0;
        this.freeBlocks = poolSize;
        console.log(`Memory pool '${name}' created with ${poolSize} blocks`);
    }
    
    ~MemoryPool() {
        console.log(`Destroying memory pool '${this.name}'`);
        console.log(`Final state: ${this.allocatedBlocks} allocated, ${this.freeBlocks} free`);
        if (this.allocatedBlocks > 0) {
            console.log("WARNING: Memory pool destroyed with allocated blocks!");
        }
    }
    
    getName(): string {
        return this.name;
    }
    
    getPoolSize(): number {
        return this.poolSize;
    }
    
    getAllocatedBlocks(): number {
        return this.allocatedBlocks;
    }
    
    getFreeBlocks(): number {
        return this.freeBlocks;
    }
    
    allocateBlock(): boolean {
        if (this.freeBlocks > 0) {
            this.freeBlocks--;
            this.allocatedBlocks++;
            console.log(`Block allocated from pool '${this.name}' (${this.allocatedBlocks}/${this.poolSize})`);
            return true;
        }
        return false;
    }
    
    deallocateBlock(): boolean {
        if (this.allocatedBlocks > 0) {
            this.allocatedBlocks--;
            this.freeBlocks++;
            console.log(`Block deallocated from pool '${this.name}' (${this.allocatedBlocks}/${this.poolSize})`);
            return true;
        }
        return false;
    }
}

// System Test 5: Cache Management
class CacheManager {
    private cacheSize: number;
    private currentSize: number;
    private hitCount: number;
    private missCount: number;
    private name: string;
    
    constructor(name: string, cacheSize: number) {
        this.name = name;
        this.cacheSize = cacheSize;
        this.currentSize = 0;
        this.hitCount = 0;
        this.missCount = 0;
        console.log(`Cache '${name}' created with size ${cacheSize}`);
    }
    
    ~CacheManager() {
        console.log(`Destroying cache '${this.name}'`);
        console.log(`Final stats: ${this.hitCount} hits, ${this.missCount} misses`);
        console.log(`Hit rate: ${this.hitCount / (this.hitCount + this.missCount) * 100}%`);
    }
    
    getName(): string {
        return this.name;
    }
    
    getCacheSize(): number {
        return this.cacheSize;
    }
    
    getCurrentSize(): number {
        return this.currentSize;
    }
    
    getHitCount(): number {
        return this.hitCount;
    }
    
    getMissCount(): number {
        return this.missCount;
    }
    
    addItem(key: string, size: number): boolean {
        if (this.currentSize + size <= this.cacheSize) {
            this.currentSize += size;
            console.log(`Item '${key}' added to cache '${this.name}' (size: ${size})`);
            return true;
        }
        return false;
    }
    
    removeItem(key: string, size: number): void {
        this.currentSize -= size;
        console.log(`Item '${key}' removed from cache '${this.name}' (size: ${size})`);
    }
    
    getItem(key: string): boolean {
        // Simulate cache hit/miss
        if (Math.random() > 0.5) {
            this.hitCount++;
            console.log(`Cache hit for '${key}' in '${this.name}'`);
            return true;
        } else {
            this.missCount++;
            console.log(`Cache miss for '${key}' in '${this.name}'`);
            return false;
        }
    }
}

// System Test 6: Thread Pool Management
class ThreadPool {
    private poolSize: number;
    private activeThreads: number;
    private completedTasks: number;
    private name: string;
    
    constructor(name: string, poolSize: number) {
        this.name = name;
        this.poolSize = poolSize;
        this.activeThreads = 0;
        this.completedTasks = 0;
        console.log(`Thread pool '${name}' created with ${poolSize} threads`);
    }
    
    ~ThreadPool() {
        console.log(`Destroying thread pool '${this.name}'`);
        console.log(`Final stats: ${this.activeThreads} active threads, ${this.completedTasks} completed tasks`);
        if (this.activeThreads > 0) {
            console.log("WARNING: Thread pool destroyed with active threads!");
        }
    }
    
    getName(): string {
        return this.name;
    }
    
    getPoolSize(): number {
        return this.poolSize;
    }
    
    getActiveThreads(): number {
        return this.activeThreads;
    }
    
    getCompletedTasks(): number {
        return this.completedTasks;
    }
    
    submitTask(taskName: string): boolean {
        if (this.activeThreads < this.poolSize) {
            this.activeThreads++;
            console.log(`Task '${taskName}' submitted to thread pool '${this.name}'`);
            return true;
        }
        return false;
    }
    
    completeTask(taskName: string): void {
        if (this.activeThreads > 0) {
            this.activeThreads--;
            this.completedTasks++;
            console.log(`Task '${taskName}' completed in thread pool '${this.name}'`);
        }
    }
}

// System Test 7: Complex Object Graph Management
class ObjectGraphNode {
    private id: string;
    private children: ObjectGraphNode[];
    private parent: ObjectGraphNode | null;
    private data: string;
    
    constructor(id: string, data: string) {
        this.id = id;
        this.data = data;
        this.children = [];
        this.parent = null;
        console.log(`Object graph node '${id}' created`);
    }
    
    ~ObjectGraphNode() {
        console.log(`Destroying object graph node '${this.id}'`);
        // Clean up children
        for (let child of this.children) {
            child.setParent(null);
        }
        this.children = [];
        
        // Remove from parent
        if (this.parent) {
            this.parent.removeChild(this);
        }
    }
    
    getId(): string {
        return this.id;
    }
    
    getData(): string {
        return this.data;
    }
    
    getChildren(): ObjectGraphNode[] {
        return this.children;
    }
    
    getParent(): ObjectGraphNode | null {
        return this.parent;
    }
    
    setParent(parent: ObjectGraphNode | null): void {
        this.parent = parent;
    }
    
    addChild(child: ObjectGraphNode): void {
        this.children.push(child);
        child.setParent(this);
        console.log(`Added child '${child.getId()}' to '${this.id}'`);
    }
    
    removeChild(child: ObjectGraphNode): void {
        let index = this.children.indexOf(child);
        if (index > -1) {
            this.children.splice(index, 1);
            console.log(`Removed child '${child.getId()}' from '${this.id}'`);
        }
    }
}

// System Test 8: Resource Manager with Multiple Resources
class ResourceManager {
    private name: string;
    private resources: Map<string, any>;
    private resourceCount: number;
    
    constructor(name: string) {
        this.name = name;
        this.resources = new Map();
        this.resourceCount = 0;
        console.log(`Resource manager '${name}' created`);
    }
    
    ~ResourceManager() {
        console.log(`Destroying resource manager '${this.name}'`);
        console.log(`Final resource count: ${this.resourceCount}`);
        
        // Clean up all resources
        for (let [key, resource] of this.resources) {
            console.log(`Cleaning up resource '${key}'`);
        }
        this.resources.clear();
    }
    
    getName(): string {
        return this.name;
    }
    
    getResourceCount(): number {
        return this.resourceCount;
    }
    
    addResource(key: string, resource: any): void {
        this.resources.set(key, resource);
        this.resourceCount++;
        console.log(`Resource '${key}' added to manager '${this.name}'`);
    }
    
    removeResource(key: string): void {
        if (this.resources.has(key)) {
            this.resources.delete(key);
            this.resourceCount--;
            console.log(`Resource '${key}' removed from manager '${this.name}'`);
        }
    }
    
    getResource(key: string): any {
        return this.resources.get(key);
    }
}

// System test functions
function testFileSystemManagement(): void {
    console.log("=== Testing File System Management ===");
    let file1 = new FileHandle("document.txt");
    let file2 = new FileHandle("image.jpg");
    
    file1.setFileSize(1024);
    file2.setFileSize(2048);
    
    console.log(`File1: ${file1.getFilename()}, size: ${file1.getFileSize()}`);
    console.log(`File2: ${file2.getFilename()}, size: ${file2.getFileSize()}`);
}

function testDatabaseManagement(): void {
    console.log("=== Testing Database Management ===");
    let db = new DatabaseConnection("postgresql://localhost:5432/mydb");
    
    db.beginTransaction();
    db.beginTransaction();
    db.commitTransaction();
    db.commitTransaction();
    
    console.log(`Database: ${db.getConnectionString()}`);
}

function testNetworkManagement(): void {
    console.log("=== Testing Network Management ===");
    let socket = new NetworkSocket("localhost", 8080);
    
    socket.transferData(512);
    socket.transferData(1024);
    
    console.log(`Socket: ${socket.getHost()}:${socket.getPort()}`);
}

function testMemoryPoolManagement(): void {
    console.log("=== Testing Memory Pool Management ===");
    let pool = new MemoryPool("main-pool", 100);
    
    pool.allocateBlock();
    pool.allocateBlock();
    pool.deallocateBlock();
    
    console.log(`Pool: ${pool.getName()}, size: ${pool.getPoolSize()}`);
    console.log(`Allocated: ${pool.getAllocatedBlocks()}, Free: ${pool.getFreeBlocks()}`);
}

function testCacheManagement(): void {
    console.log("=== Testing Cache Management ===");
    let cache = new CacheManager("lru-cache", 1000);
    
    cache.addItem("item1", 100);
    cache.addItem("item2", 200);
    cache.getItem("item1");
    cache.getItem("item2");
    cache.removeItem("item1", 100);
    
    console.log(`Cache: ${cache.getName()}, size: ${cache.getCacheSize()}`);
    console.log(`Current size: ${cache.getCurrentSize()}`);
}

function testThreadPoolManagement(): void {
    console.log("=== Testing Thread Pool Management ===");
    let pool = new ThreadPool("worker-pool", 4);
    
    pool.submitTask("task1");
    pool.submitTask("task2");
    pool.completeTask("task1");
    pool.completeTask("task2");
    
    console.log(`Thread pool: ${pool.getName()}, size: ${pool.getPoolSize()}`);
    console.log(`Active threads: ${pool.getActiveThreads()}`);
}

function testObjectGraphManagement(): void {
    console.log("=== Testing Object Graph Management ===");
    let root = new ObjectGraphNode("root", "root-data");
    let child1 = new ObjectGraphNode("child1", "child1-data");
    let child2 = new ObjectGraphNode("child2", "child2-data");
    let grandchild = new ObjectGraphNode("grandchild", "grandchild-data");
    
    root.addChild(child1);
    root.addChild(child2);
    child1.addChild(grandchild);
    
    console.log(`Root: ${root.getId()}, children: ${root.getChildren().length}`);
    console.log(`Child1: ${child1.getId()}, children: ${child1.getChildren().length}`);
}

function testResourceManagerManagement(): void {
    console.log("=== Testing Resource Manager Management ===");
    let manager = new ResourceManager("main-manager");
    
    let file = new FileHandle("managed-file.txt");
    let socket = new NetworkSocket("localhost", 9090);
    
    manager.addResource("file", file);
    manager.addResource("socket", socket);
    
    console.log(`Manager: ${manager.getName()}, resources: ${manager.getResourceCount()}`);
}

// Run all system tests
function runAllSystemTests(): void {
    console.log("🚀 Starting RAII System Tests");
    console.log("===============================");
    
    testFileSystemManagement();
    testDatabaseManagement();
    testNetworkManagement();
    testMemoryPoolManagement();
    testCacheManagement();
    testThreadPoolManagement();
    testObjectGraphManagement();
    testResourceManagerManagement();
    
    console.log("===============================");
    console.log("✅ All RAII System Tests Completed");
}

// Export for testing
export { runAllSystemTests };