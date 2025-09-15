// Systems Programming with TypeScript
// Demonstrates TypeScript for systems programming, API servers, CLI apps, and drivers

// 1. CLI Application Framework
class CLIApplication {
  private commands = new Map<string, Command>();
  private options = new Map<string, Option>();

  addCommand(name: string, description: string, handler: (args: string[]) => void): void {
    this.commands.set(name, { description, handler });
  }

  addOption(short: string, long: string, description: string, required: boolean = false): void {
    this.options.set(short, { long, description, required });
  }

  parseArgs(args: string[]): ParsedArgs {
    const parsed: ParsedArgs = {
      command: "",
      options: new Map(),
      arguments: []
    };

    let i = 0;
    while (i < args.length) {
      const arg = args[i];
      
      if (arg.startsWith("--")) {
        const optionName = arg.substring(2);
        const option = this.findOptionByLong(optionName);
        if (option) {
          parsed.options.set(optionName, args[++i] || "true");
        }
      } else if (arg.startsWith("-")) {
        const optionName = arg.substring(1);
        const option = this.options.get(optionName);
        if (option) {
          parsed.options.set(option.long, args[++i] || "true");
        }
      } else if (parsed.command === "") {
        parsed.command = arg;
      } else {
        parsed.arguments.push(arg);
      }
      i++;
    }

    return parsed;
  }

  private findOptionByLong(long: string): Option | undefined {
    for (const option of this.options.values()) {
      if (option.long === long) {
        return option;
      }
    }
    return undefined;
  }

  run(args: string[]): void {
    const parsed = this.parseArgs(args);
    const command = this.commands.get(parsed.command);
    
    if (command) {
      command.handler(parsed.arguments);
    } else {
      this.showHelp();
    }
  }

  private showHelp(): void {
    _print("Available commands:");
    for (const [name, cmd] of this.commands) {
      _print(`  ${name}: ${cmd.description}`);
    }
    
    _print("\nAvailable options:");
    for (const [short, opt] of this.options) {
      _print(`  -${short}, --${opt.long}: ${opt.description}`);
    }
  }
}

interface Command {
  description: string;
  handler: (args: string[]) => void;
}

interface Option {
  long: string;
  description: string;
  required: boolean;
}

interface ParsedArgs {
  command: string;
  options: Map<string, string>;
  arguments: string[];
}

// 2. HTTP Server Framework
class HTTPServer {
  private routes = new Map<string, RouteHandler>();
  private middleware: Middleware[] = [];

  get(path: string, handler: RouteHandler): void {
    this.routes.set(`GET:${path}`, handler);
  }

  post(path: string, handler: RouteHandler): void {
    this.routes.set(`POST:${path}`, handler);
  }

  put(path: string, handler: RouteHandler): void {
    this.routes.set(`PUT:${path}`, handler);
  }

  delete(path: string, handler: RouteHandler): void {
    this.routes.set(`DELETE:${path}`, handler);
  }

  use(middleware: Middleware): void {
    this.middleware.push(middleware);
  }

  async handleRequest(req: HTTPRequest): Promise<HTTPResponse> {
    // Apply middleware
    for (const middleware of this.middleware) {
      const result = await middleware(req);
      if (result) {
        return result;
      }
    }

    // Find route handler
    const routeKey = `${req.method}:${req.path}`;
    const handler = this.routes.get(routeKey);
    
    if (handler) {
      return await handler(req);
    }

    return {
      status: 404,
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ error: "Not Found" })
    };
  }

  listen(port: number, callback?: () => void): void {
    _print(`Server listening on port ${port}`);
    if (callback) callback();
  }
}

type RouteHandler = (req: HTTPRequest) => Promise<HTTPResponse>;
type Middleware = (req: HTTPRequest) => Promise<HTTPResponse | null>;

interface HTTPRequest {
  method: string;
  path: string;
  headers: Record<string, string>;
  body: string;
  query: Record<string, string>;
}

interface HTTPResponse {
  status: number;
  headers: Record<string, string>;
  body: string;
}

// 3. Database ORM
class Database {
  private connection: Connection | null = null;

  async connect(connectionString: string): Promise<void> {
    _print(`Connecting to database: ${connectionString}`);
    this.connection = new Connection(connectionString);
  }

  async query<T>(sql: string, params: any[] = []): Promise<T[]> {
    if (!this.connection) {
      throw new Error("Database not connected");
    }
    return this.connection.execute<T>(sql, params);
  }

  async transaction<T>(callback: (db: Database) => Promise<T>): Promise<T> {
    if (!this.connection) {
      throw new Error("Database not connected");
    }

    await this.connection.beginTransaction();
    try {
      const result = await callback(this);
      await this.connection.commit();
      return result;
    } catch (error) {
      await this.connection.rollback();
      throw error;
    }
  }
}

class Connection {
  constructor(private connectionString: string) {}

  async execute<T>(sql: string, params: any[]): Promise<T[]> {
    _print(`Executing SQL: ${sql} with params:`, params);
    // Simulate database execution
    return [] as T[];
  }

  async beginTransaction(): Promise<void> {
    _print("Beginning transaction");
  }

  async commit(): Promise<void> {
    _print("Committing transaction");
  }

  async rollback(): Promise<void> {
    _print("Rolling back transaction");
  }
}

// 4. Configuration Management
class ConfigManager {
  private config = new Map<string, any>();
  private envPrefix = "";

  constructor(envPrefix: string = "") {
    this.envPrefix = envPrefix;
  }

  loadFromEnv(): void {
    // Simulate environment variable loading
    this.config.set("database.host", "localhost");
    this.config.set("database.port", 5432);
    this.config.set("server.port", 3000);
    this.config.set("server.host", "0.0.0.0");
  }

  loadFromFile(filename: string): void {
    _print(`Loading config from ${filename}`);
    // Simulate file loading
    this.config.set("app.name", "TypeScript Server");
    this.config.set("app.version", "1.0.0");
  }

  get<T>(key: string, defaultValue?: T): T | undefined {
    return this.config.get(key) as T || defaultValue;
  }

  set<T>(key: string, value: T): void {
    this.config.set(key, value);
  }

  has(key: string): boolean {
    return this.config.has(key);
  }
}

// 5. Logging System
enum LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARN = 2,
  ERROR = 3
}

class Logger {
  private level: LogLevel;
  private context: string;

  constructor(context: string = "app", level: LogLevel = LogLevel.INFO) {
    this.context = context;
    this.level = level;
  }

  debug(message: string, ...args: any[]): void {
    if (this.level <= LogLevel.DEBUG) {
      this.log("DEBUG", message, ...args);
    }
  }

  info(message: string, ...args: any[]): void {
    if (this.level <= LogLevel.INFO) {
      this.log("INFO", message, ...args);
    }
  }

  warn(message: string, ...args: any[]): void {
    if (this.level <= LogLevel.WARN) {
      this.log("WARN", message, ...args);
    }
  }

  error(message: string, ...args: any[]): void {
    if (this.level <= LogLevel.ERROR) {
      this.log("ERROR", message, ...args);
    }
  }

  private log(level: string, message: string, ...args: any[]): void {
    const timestamp = new Date().toISOString();
    _print(`[${timestamp}] [${level}] [${this.context}] ${message}`, ...args);
  }
}

// 6. File System Operations
class FileSystem {
  async readFile(filename: string): Promise<string> {
    _print(`Reading file: ${filename}`);
    // Simulate file reading
    return `Content of ${filename}`;
  }

  async writeFile(filename: string, content: string): Promise<void> {
    _print(`Writing file: ${filename}`);
    // Simulate file writing
  }

  async exists(filename: string): Promise<boolean> {
    _print(`Checking if file exists: ${filename}`);
    // Simulate file existence check
    return true;
  }

  async mkdir(path: string, recursive: boolean = false): Promise<void> {
    _print(`Creating directory: ${path} (recursive: ${recursive})`);
    // Simulate directory creation
  }

  async readdir(path: string): Promise<string[]> {
    _print(`Reading directory: ${path}`);
    // Simulate directory reading
    return ["file1.txt", "file2.txt", "subdir"];
  }
}

// 7. Process Management
class ProcessManager {
  private processes = new Map<number, Process>();

  async spawn(command: string, args: string[] = [], options: SpawnOptions = {}): Promise<Process> {
    const pid = Math.floor(Math.random() * 10000) + 1000; // Simulate PID
    const process = new Process(pid, command, args, options);
    this.processes.set(pid, process);
    
    _print(`Spawned process ${pid}: ${command} ${args.join(" ")}`);
    return process;
  }

  async kill(pid: number, signal: string = "SIGTERM"): Promise<void> {
    const process = this.processes.get(pid);
    if (process) {
      await process.kill(signal);
      this.processes.delete(pid);
    }
  }

  async wait(pid: number): Promise<number> {
    const process = this.processes.get(pid);
    if (process) {
      return await process.wait();
    }
    return -1;
  }
}

class Process {
  constructor(
    public pid: number,
    public command: string,
    public args: string[],
    public options: SpawnOptions
  ) {}

  async kill(signal: string): Promise<void> {
    _print(`Killing process ${this.pid} with signal ${signal}`);
  }

  async wait(): Promise<number> {
    _print(`Waiting for process ${this.pid} to exit`);
    return 0; // Simulate exit code
  }
}

interface SpawnOptions {
  cwd?: string;
  env?: Record<string, string>;
  stdio?: "inherit" | "pipe" | "ignore";
}

// 8. Memory Management
class MemoryManager {
  private allocations = new Map<number, Allocation>();
  private nextId = 1;

  allocate(size: number): number {
    const id = this.nextId++;
    const allocation: Allocation = {
      id,
      size,
      address: Math.floor(Math.random() * 0xFFFFFFFF),
      timestamp: Date.now()
    };
    
    this.allocations.set(id, allocation);
    _print(`Allocated ${size} bytes at address 0x${allocation.address.toString(16)}`);
    return id;
  }

  deallocate(id: number): void {
    const allocation = this.allocations.get(id);
    if (allocation) {
      _print(`Deallocated ${allocation.size} bytes at address 0x${allocation.address.toString(16)}`);
      this.allocations.delete(id);
    }
  }

  getStats(): MemoryStats {
    let totalAllocated = 0;
    let totalCount = 0;

    for (const allocation of this.allocations.values()) {
      totalAllocated += allocation.size;
      totalCount++;
    }

    return {
      totalAllocated,
      totalCount,
      activeAllocations: Array.from(this.allocations.values())
    };
  }
}

interface Allocation {
  id: number;
  size: number;
  address: number;
  timestamp: number;
}

interface MemoryStats {
  totalAllocated: number;
  totalCount: number;
  activeAllocations: Allocation[];
}

// 9. Network Operations
class NetworkManager {
  async tcpConnect(host: string, port: number): Promise<TCPSocket> {
    _print(`Connecting to ${host}:${port}`);
    return new TCPSocket(host, port);
  }

  async udpBind(port: number): Promise<UDPSocket> {
    _print(`Binding UDP socket to port ${port}`);
    return new UDPSocket(port);
  }

  async resolveHostname(hostname: string): Promise<string[]> {
    _print(`Resolving hostname: ${hostname}`);
    // Simulate DNS resolution
    return ["127.0.0.1", "::1"];
  }
}

class TCPSocket {
  constructor(private host: string, private port: number) {}

  async send(data: string): Promise<void> {
    _print(`Sending data to ${this.host}:${this.port}: ${data}`);
  }

  async receive(): Promise<string> {
    _print(`Receiving data from ${this.host}:${this.port}`);
    return "Received data";
  }

  async close(): Promise<void> {
    _print(`Closing connection to ${this.host}:${this.port}`);
  }
}

class UDPSocket {
  constructor(private port: number) {}

  async send(data: string, host: string, port: number): Promise<void> {
    _print(`Sending UDP data to ${host}:${port}: ${data}`);
  }

  async receive(): Promise<{ data: string; from: { host: string; port: number } }> {
    _print(`Receiving UDP data on port ${this.port}`);
    return { data: "Received data", from: { host: "127.0.0.1", port: 12345 } };
  }

  async close(): Promise<void> {
    _print(`Closing UDP socket on port ${this.port}`);
  }
}

// 10. System Information
class SystemInfo {
  async getCPUInfo(): Promise<CPUInfo> {
    return {
      cores: 8,
      model: "Intel Core i7",
      frequency: 3200,
      architecture: "x86_64"
    };
  }

  async getMemoryInfo(): Promise<MemoryInfo> {
    return {
      total: 16 * 1024 * 1024 * 1024, // 16GB
      free: 8 * 1024 * 1024 * 1024,    // 8GB
      used: 8 * 1024 * 1024 * 1024     // 8GB
    };
  }

  async getDiskInfo(): Promise<DiskInfo[]> {
    return [
      {
        device: "/dev/sda",
        mountpoint: "/",
        total: 500 * 1024 * 1024 * 1024, // 500GB
        free: 200 * 1024 * 1024 * 1024,  // 200GB
        used: 300 * 1024 * 1024 * 1024   // 300GB
      }
    ];
  }
}

interface CPUInfo {
  cores: number;
  model: string;
  frequency: number;
  architecture: string;
}

interface MemoryInfo {
  total: number;
  free: number;
  used: number;
}

interface DiskInfo {
  device: string;
  mountpoint: string;
  total: number;
  free: number;
  used: number;
}

// Usage Examples
async function demonstrateSystemsProgramming(): Promise<void> {
  _print("=== TypeScript Systems Programming Demo ===\n");

  // 1. CLI Application
  _print("1. CLI Application:");
  const cli = new CLIApplication();
  cli.addCommand("serve", "Start the server", (args) => {
    _print("Starting server with args:", args);
  });
  cli.addCommand("build", "Build the project", (args) => {
    _print("Building project with args:", args);
  });
  cli.addOption("p", "port", "Port number", true);
  cli.addOption("h", "help", "Show help");

  cli.run(["serve", "--port", "3000", "arg1", "arg2"]);

  // 2. HTTP Server
  _print("\n2. HTTP Server:");
  const server = new HTTPServer();
  
  server.get("/api/users", async (req) => ({
    status: 200,
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify([{ id: 1, name: "Alice" }])
  }));

  server.post("/api/users", async (req) => ({
    status: 201,
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ id: 2, name: "Bob" })
  }));

  server.use(async (req) => {
    _print(`Middleware: ${req.method} ${req.path}`);
    return null; // Continue to next middleware/handler
  });

  await server.handleRequest({
    method: "GET",
    path: "/api/users",
    headers: {},
    body: "",
    query: {}
  });

  server.listen(3000);

  // 3. Database Operations
  _print("\n3. Database Operations:");
  const db = new Database();
  await db.connect("postgresql://localhost:5432/mydb");
  
  await db.transaction(async (tx) => {
    await tx.query("INSERT INTO users (name) VALUES ($1)", ["Alice"]);
    await tx.query("INSERT INTO users (name) VALUES ($2)", ["Bob"]);
  });

  // 4. Configuration Management
  _print("\n4. Configuration Management:");
  const config = new ConfigManager("APP_");
  config.loadFromEnv();
  config.loadFromFile("config.json");
  
  _print("Database host:", config.get("database.host"));
  _print("Server port:", config.get("server.port"));

  // 5. Logging
  _print("\n5. Logging System:");
  const logger = new Logger("server", LogLevel.DEBUG);
  logger.debug("Debug message");
  logger.info("Info message");
  logger.warn("Warning message");
  logger.error("Error message");

  // 6. File System
  _print("\n6. File System Operations:");
  const fs = new FileSystem();
  await fs.writeFile("output.txt", "Hello, World!");
  const content = await fs.readFile("output.txt");
  _print("File content:", content);

  // 7. Process Management
  _print("\n7. Process Management:");
  const processManager = new ProcessManager();
  const process = await processManager.spawn("ls", ["-la"], { cwd: "/tmp" });
  await processManager.kill(process.pid);

  // 8. Memory Management
  _print("\n8. Memory Management:");
  const memoryManager = new MemoryManager();
  const alloc1 = memoryManager.allocate(1024);
  const alloc2 = memoryManager.allocate(2048);
  
  const stats = memoryManager.getStats();
  _print("Memory stats:", stats);
  
  memoryManager.deallocate(alloc1);
  memoryManager.deallocate(alloc2);

  // 9. Network Operations
  _print("\n9. Network Operations:");
  const networkManager = new NetworkManager();
  const tcpSocket = await networkManager.tcpConnect("localhost", 8080);
  await tcpSocket.send("Hello, Server!");
  await tcpSocket.close();

  const udpSocket = await networkManager.udpBind(9090);
  await udpSocket.send("Hello, UDP!", "localhost", 8080);
  await udpSocket.close();

  // 10. System Information
  _print("\n10. System Information:");
  const systemInfo = new SystemInfo();
  const cpuInfo = await systemInfo.getCPUInfo();
  const memoryInfo = await systemInfo.getMemoryInfo();
  const diskInfo = await systemInfo.getDiskInfo();
  
  _print("CPU:", cpuInfo);
  _print("Memory:", memoryInfo);
  _print("Disk:", diskInfo);
}

// This example demonstrates TypeScript as a systems programming language
// capable of building API servers, CLI applications, and system utilities