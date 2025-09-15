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
        _print(`File '${filename}' opened`);
    }
    
    ~FileHandle() {
        if (this.isOpen) {
            _print(`Closing file '${this.filename}' (size: ${this.fileSize} bytes)`);
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
        _print(`Database connection established: ${connectionString}`);
    }
    
    ~DatabaseConnection() {
        if (this.isConnected) {
            _print(`Closing database connection: ${this.connectionString}`);
            _print(`Final transaction count: ${this.transactionCount}`);
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
            _print(`Transaction ${this.transactionCount} begun`);
        }
    }
    
    commitTransaction(): void {
        if (this.isConnected && this.transactionCount > 0) {
            _print(`Transaction ${this.transactionCount} committed`);
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
        _print(`Socket connected to ${host}:${port}`);
    }
    
    ~NetworkSocket() {
        if (this.isConnected) {
            _print(`Closing socket to ${this.host}:${this.port}`);
            _print(`Total bytes transferred: ${this.bytesTransferred}`);
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
            _print(`Transferred ${bytes} bytes (total: ${this.bytesTransferred})`);
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
        _print(`Memory pool '${name}' created with ${poolSize} blocks`);
    }
    
    ~MemoryPool() {
        _print(`Destroying memory pool '${this.name}'`);
        _print(`Final state: ${this.allocatedBlocks} allocated, ${this.freeBlocks} free`);
        if (this.allocatedBlocks > 0) {
            _print("WARNING: Memory pool destroyed with allocated blocks!");
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
            _print(`Block allocated from pool '${this.name}' (${this.allocatedBlocks}/${this.poolSize})`);
            return true;
        }
        return false;
    }
    
    deallocateBlock(): boolean {
        if (this.allocatedBlocks > 0) {
            this.allocatedBlocks--;
            this.freeBlocks++;
            _print(`Block deallocated from pool '${this.name}' (${this.allocatedBlocks}/${this.poolSize})`);
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
        _print(`Cache '${name}' created with size ${cacheSize}`);
    }
    
    ~CacheManager() {
        _print(`Destroying cache '${this.name}'`);
        _print(`Final stats: ${this.hitCount} hits, ${this.missCount} misses`);
        _print(`Hit rate: ${this.hitCount / (this.hitCount + this.missCount) * 100}%`);
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
            _print(`Item '${key}' added to cache '${this.name}' (size: ${size})`);
            return true;
        }
        return false;
    }
    
    removeItem(key: string, size: number): void {
        this.currentSize -= size;
        _print(`Item '${key}' removed from cache '${this.name}' (size: ${size})`);
    }
    
    getItem(key: string): boolean {
        // Simulate cache hit/miss
        if (Math.random() > 0.5) {
            this.hitCount++;
            _print(`Cache hit for '${key}' in '${this.name}'`);
            return true;
        } else {
            this.missCount++;
            _print(`Cache miss for '${key}' in '${this.name}'`);
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
        _print(`Thread pool '${name}' created with ${poolSize} threads`);
    }
    
    ~ThreadPool() {
        _print(`Destroying thread pool '${this.name}'`);
        _print(`Final stats: ${this.activeThreads} active threads, ${this.completedTasks} completed tasks`);
        if (this.activeThreads > 0) {
            _print("WARNING: Thread pool destroyed with active threads!");
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
            _print(`Task '${taskName}' submitted to thread pool '${this.name}'`);
            return true;
        }
        return false;
    }
    
    completeTask(taskName: string): void {
        if (this.activeThreads > 0) {
            this.activeThreads--;
            this.completedTasks++;
            _print(`Task '${taskName}' completed in thread pool '${this.name}'`);
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
        _print(`Object graph node '${id}' created`);
    }
    
    ~ObjectGraphNode() {
        _print(`Destroying object graph node '${this.id}'`);
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
        _print(`Added child '${child.getId()}' to '${this.id}'`);
    }
    
    removeChild(child: ObjectGraphNode): void {
        let index = this.children.indexOf(child);
        if (index > -1) {
            this.children.splice(index, 1);
            _print(`Removed child '${child.getId()}' from '${this.id}'`);
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
        _print(`Resource manager '${name}' created`);
    }
    
    ~ResourceManager() {
        _print(`Destroying resource manager '${this.name}'`);
        _print(`Final resource count: ${this.resourceCount}`);
        
        // Clean up all resources
        for (let [key, resource] of this.resources) {
            _print(`Cleaning up resource '${key}'`);
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
        _print(`Resource '${key}' added to manager '${this.name}'`);
    }
    
    removeResource(key: string): void {
        if (this.resources.has(key)) {
            this.resources.delete(key);
            this.resourceCount--;
            _print(`Resource '${key}' removed from manager '${this.name}'`);
        }
    }
    
    getResource(key: string): any {
        return this.resources.get(key);
    }
}

// System test functions
function testFileSystemManagement(): void {
    _print("=== Testing File System Management ===");
    let file1 = new FileHandle("document.txt");
    let file2 = new FileHandle("image.jpg");
    
    file1.setFileSize(1024);
    file2.setFileSize(2048);
    
    _print(`File1: ${file1.getFilename()}, size: ${file1.getFileSize()}`);
    _print(`File2: ${file2.getFilename()}, size: ${file2.getFileSize()}`);
}

function testDatabaseManagement(): void {
    _print("=== Testing Database Management ===");
    let db = new DatabaseConnection("postgresql://localhost:5432/mydb");
    
    db.beginTransaction();
    db.beginTransaction();
    db.commitTransaction();
    db.commitTransaction();
    
    _print(`Database: ${db.getConnectionString()}`);
}

function testNetworkManagement(): void {
    _print("=== Testing Network Management ===");
    let socket = new NetworkSocket("localhost", 8080);
    
    socket.transferData(512);
    socket.transferData(1024);
    
    _print(`Socket: ${socket.getHost()}:${socket.getPort()}`);
}

function testMemoryPoolManagement(): void {
    _print("=== Testing Memory Pool Management ===");
    let pool = new MemoryPool("main-pool", 100);
    
    pool.allocateBlock();
    pool.allocateBlock();
    pool.deallocateBlock();
    
    _print(`Pool: ${pool.getName()}, size: ${pool.getPoolSize()}`);
    _print(`Allocated: ${pool.getAllocatedBlocks()}, Free: ${pool.getFreeBlocks()}`);
}

function testCacheManagement(): void {
    _print("=== Testing Cache Management ===");
    let cache = new CacheManager("lru-cache", 1000);
    
    cache.addItem("item1", 100);
    cache.addItem("item2", 200);
    cache.getItem("item1");
    cache.getItem("item2");
    cache.removeItem("item1", 100);
    
    _print(`Cache: ${cache.getName()}, size: ${cache.getCacheSize()}`);
    _print(`Current size: ${cache.getCurrentSize()}`);
}

function testThreadPoolManagement(): void {
    _print("=== Testing Thread Pool Management ===");
    let pool = new ThreadPool("worker-pool", 4);
    
    pool.submitTask("task1");
    pool.submitTask("task2");
    pool.completeTask("task1");
    pool.completeTask("task2");
    
    _print(`Thread pool: ${pool.getName()}, size: ${pool.getPoolSize()}`);
    _print(`Active threads: ${pool.getActiveThreads()}`);
}

function testObjectGraphManagement(): void {
    _print("=== Testing Object Graph Management ===");
    let root = new ObjectGraphNode("root", "root-data");
    let child1 = new ObjectGraphNode("child1", "child1-data");
    let child2 = new ObjectGraphNode("child2", "child2-data");
    let grandchild = new ObjectGraphNode("grandchild", "grandchild-data");
    
    root.addChild(child1);
    root.addChild(child2);
    child1.addChild(grandchild);
    
    _print(`Root: ${root.getId()}, children: ${root.getChildren().length}`);
    _print(`Child1: ${child1.getId()}, children: ${child1.getChildren().length}`);
}

function testResourceManagerManagement(): void {
    _print("=== Testing Resource Manager Management ===");
    let manager = new ResourceManager("main-manager");
    
    let file = new FileHandle("managed-file.txt");
    let socket = new NetworkSocket("localhost", 9090);
    
    manager.addResource("file", file);
    manager.addResource("socket", socket);
    
    _print(`Manager: ${manager.getName()}, resources: ${manager.getResourceCount()}`);
}

// Run all system tests
function runAllSystemTests(): void {
    _print("🚀 Starting RAII System Tests");
    _print("===============================");
    
    testFileSystemManagement();
    testDatabaseManagement();
    testNetworkManagement();
    testMemoryPoolManagement();
    testCacheManagement();
    testThreadPoolManagement();
    testObjectGraphManagement();
    testResourceManagerManagement();
    
    _print("===============================");
    _print("✅ All RAII System Tests Completed");
}

// Export for testing
export { runAllSystemTests };