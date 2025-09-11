// Async/Await
// Demonstrates asynchronous programming with TypeScript type safety

// Basic async/await
function delay(ms: number): Promise<string> {
  return new Promise(resolve => {
    setTimeout(() => resolve(`Done after ${ms}ms`), ms);
  });
}

async function runTask(): Promise<void> {
  console.log("Task started...");
  const result = await delay(1000);
  console.log(result);
}

// Error handling with try/catch
async function fetchData(): Promise<string> {
  // Simulate API call
  await delay(500);
  return "Hello World";
}

async function fetchDataWithError(): Promise<string> {
  await delay(500);
  throw new Error("API request failed");
}

async function main(): Promise<void> {
  try {
    const data = await fetchData();
    console.log("Data:", data);
  } catch (err) {
    console.error("Error:", err);
  }

  try {
    const errorData = await fetchDataWithError();
    console.log("This won't be reached");
  } catch (err) {
    console.error("Caught error:", err);
  }
}

// Async function with return types
async function getUser(id: number): Promise<{ id: number; name: string; email: string }> {
  await delay(200);
  return {
    id,
    name: `User ${id}`,
    email: `user${id}@example.com`
  };
}

async function getUsers(ids: number[]): Promise<{ id: number; name: string; email: string }[]> {
  const users = await Promise.all(ids.map(id => getUser(id)));
  return users;
}

// Async iteration
async function* asyncGenerator(): AsyncGenerator<string, void, unknown> {
  yield "First";
  await delay(100);
  yield "Second";
  await delay(100);
  yield "Third";
}

async function processAsyncGenerator(): Promise<void> {
  console.log("Processing async generator:");
  for await (const value of asyncGenerator()) {
    console.log("Received:", value);
  }
}

// Async class methods
class ApiClient {
  private baseUrl: string;

  constructor(baseUrl: string) {
    this.baseUrl = baseUrl;
  }

  async get<T>(endpoint: string): Promise<T> {
    console.log(`GET ${this.baseUrl}${endpoint}`);
    await delay(300);
    return {} as T;
  }

  async post<T>(endpoint: string, data: any): Promise<T> {
    console.log(`POST ${this.baseUrl}${endpoint}`, data);
    await delay(400);
    return {} as T;
  }

  async put<T>(endpoint: string, data: any): Promise<T> {
    console.log(`PUT ${this.baseUrl}${endpoint}`, data);
    await delay(350);
    return {} as T;
  }

  async delete<T>(endpoint: string): Promise<T> {
    console.log(`DELETE ${this.baseUrl}${endpoint}`);
    await delay(250);
    return {} as T;
  }
}

// Promise utilities
class PromiseUtils {
  static async timeout<T>(promise: Promise<T>, ms: number): Promise<T> {
    const timeoutPromise = new Promise<never>((_, reject) => {
      setTimeout(() => reject(new Error(`Timeout after ${ms}ms`)), ms);
    });

    return Promise.race([promise, timeoutPromise]);
  }

  static async retry<T>(
    fn: () => Promise<T>,
    maxRetries: number = 3,
    delayMs: number = 1000
  ): Promise<T> {
    let lastError: Error;

    for (let attempt = 1; attempt <= maxRetries; attempt++) {
      try {
        return await fn();
      } catch (error) {
        lastError = error as Error;
        console.log(`Attempt ${attempt} failed:`, error);
        
        if (attempt < maxRetries) {
          await delay(delayMs);
        }
      }
    }

    throw lastError!;
  }

  static async allSettled<T>(promises: Promise<T>[]): Promise<Array<{ status: 'fulfilled' | 'rejected'; value?: T; reason?: any }>> {
    const results = await Promise.allSettled(promises);
    return results.map(result => ({
      status: result.status,
      value: result.status === 'fulfilled' ? result.value : undefined,
      reason: result.status === 'rejected' ? result.reason : undefined
    }));
  }
}

// Async error handling patterns
class AsyncErrorHandler {
  static async safeExecute<T>(fn: () => Promise<T>): Promise<{ success: true; data: T } | { success: false; error: string }> {
    try {
      const data = await fn();
      return { success: true, data };
    } catch (error) {
      return { success: false, error: (error as Error).message };
    }
  }

  static async withFallback<T>(
    primary: () => Promise<T>,
    fallback: () => Promise<T>
  ): Promise<T> {
    try {
      return await primary();
    } catch (error) {
      console.log("Primary failed, using fallback:", error);
      return await fallback();
    }
  }
}

// Async queue implementation
class AsyncQueue<T> {
  private queue: T[] = [];
  private processing = false;
  private processor: (item: T) => Promise<void>;

  constructor(processor: (item: T) => Promise<void>) {
    this.processor = processor;
  }

  async add(item: T): Promise<void> {
    this.queue.push(item);
    if (!this.processing) {
      await this.process();
    }
  }

  private async process(): Promise<void> {
    this.processing = true;

    while (this.queue.length > 0) {
      const item = this.queue.shift()!;
      try {
        await this.processor(item);
      } catch (error) {
        console.error("Error processing item:", error);
      }
    }

    this.processing = false;
  }

  get length(): number {
    return this.queue.length;
  }

  get isProcessing(): boolean {
    return this.processing;
  }
}

// Async cache implementation
class AsyncCache<K, V> {
  private cache = new Map<K, Promise<V>>();
  private fetcher: (key: K) => Promise<V>;

  constructor(fetcher: (key: K) => Promise<V>) {
    this.fetcher = fetcher;
  }

  async get(key: K): Promise<V> {
    if (!this.cache.has(key)) {
      this.cache.set(key, this.fetcher(key));
    }
    return this.cache.get(key)!;
  }

  has(key: K): boolean {
    return this.cache.has(key);
  }

  delete(key: K): boolean {
    return this.cache.delete(key);
  }

  clear(): void {
    this.cache.clear();
  }
}

// Usage examples
async function demonstrateAsyncFeatures(): Promise<void> {
  console.log("=== Basic Async/Await ===");
  await runTask();

  console.log("\n=== Error Handling ===");
  await main();

  console.log("\n=== Async Functions with Types ===");
  const user = await getUser(1);
  console.log("User:", user);

  const users = await getUsers([1, 2, 3]);
  console.log("Users:", users);

  console.log("\n=== Async Iteration ===");
  await processAsyncGenerator();

  console.log("\n=== API Client ===");
  const apiClient = new ApiClient("https://api.example.com");
  await apiClient.get<{ id: number }>("/users/1");
  await apiClient.post<{ id: number }>("/users", { name: "Alice" });

  console.log("\n=== Promise Utilities ===");
  try {
    const result = await PromiseUtils.timeout(fetchData(), 2000);
    console.log("Timeout result:", result);
  } catch (error) {
    console.log("Timeout error:", error);
  }

  console.log("\n=== Retry Logic ===");
  try {
    const retryResult = await PromiseUtils.retry(async () => {
      if (Math.random() > 0.5) {
        throw new Error("Random failure");
      }
      return "Success!";
    }, 3, 500);
    console.log("Retry result:", retryResult);
  } catch (error) {
    console.log("Retry failed:", error);
  }

  console.log("\n=== Promise.allSettled ===");
  const promises = [
    fetchData(),
    fetchDataWithError(),
    delay(100).then(() => "Delayed result")
  ];
  const settledResults = await PromiseUtils.allSettled(promises);
  console.log("Settled results:", settledResults);

  console.log("\n=== Error Handling Patterns ===");
  const safeResult = await AsyncErrorHandler.safeExecute(async () => {
    await delay(100);
    return "Safe execution result";
  });
  console.log("Safe result:", safeResult);

  const fallbackResult = await AsyncErrorHandler.withFallback(
    async () => {
      throw new Error("Primary failed");
    },
    async () => {
      return "Fallback result";
    }
  );
  console.log("Fallback result:", fallbackResult);

  console.log("\n=== Async Queue ===");
  const queue = new AsyncQueue<number>(async (item) => {
    console.log(`Processing item: ${item}`);
    await delay(100);
  });

  await queue.add(1);
  await queue.add(2);
  await queue.add(3);

  console.log("\n=== Async Cache ===");
  const cache = new AsyncCache<string, string>(async (key) => {
    console.log(`Fetching data for key: ${key}`);
    await delay(200);
    return `Data for ${key}`;
  });

  const cachedData1 = await cache.get("key1");
  const cachedData2 = await cache.get("key1"); // Should use cache
  console.log("Cached data 1:", cachedData1);
  console.log("Cached data 2:", cachedData2);
}

// Advanced async patterns

// Async semaphore for limiting concurrent operations
class AsyncSemaphore {
  private permits: number;
  private waiting: Array<() => void> = [];

  constructor(permits: number) {
    this.permits = permits;
  }

  async acquire(): Promise<void> {
    if (this.permits > 0) {
      this.permits--;
      return;
    }

    return new Promise<void>(resolve => {
      this.waiting.push(resolve);
    });
  }

  release(): void {
    this.permits++;
    if (this.waiting.length > 0) {
      const next = this.waiting.shift()!;
      this.permits--;
      next();
    }
  }
}

// Async batch processor
class AsyncBatchProcessor<T, R> {
  private batch: T[] = [];
  private batchSize: number;
  private processor: (items: T[]) => Promise<R[]>;
  private timeout: number;
  private timeoutId?: NodeJS.Timeout;

  constructor(
    batchSize: number,
    processor: (items: T[]) => Promise<R[]>,
    timeout: number = 1000
  ) {
    this.batchSize = batchSize;
    this.processor = processor;
    this.timeout = timeout;
  }

  async add(item: T): Promise<R> {
    return new Promise<R>((resolve, reject) => {
      this.batch.push(item);
      
      if (this.batch.length >= this.batchSize) {
        this.processBatch().then(resolve).catch(reject);
      } else {
        this.scheduleTimeout(resolve, reject);
      }
    });
  }

  private scheduleTimeout(resolve: (value: R) => void, reject: (reason?: any) => void): void {
    if (this.timeoutId) {
      clearTimeout(this.timeoutId);
    }

    this.timeoutId = setTimeout(() => {
      this.processBatch().then(resolve).catch(reject);
    }, this.timeout);
  }

  private async processBatch(): Promise<R> {
    if (this.batch.length === 0) {
      throw new Error("No items to process");
    }

    const items = this.batch.splice(0, this.batchSize);
    const results = await this.processor(items);
    
    if (this.timeoutId) {
      clearTimeout(this.timeoutId);
      this.timeoutId = undefined;
    }

    return results[0]; // Return first result for simplicity
  }
}

// This example demonstrates comprehensive async/await functionality