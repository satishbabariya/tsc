// Concurrent System Example
// Demonstrates building concurrent and parallel systems with TypeScript

// Worker thread implementation
class WorkerThread {
  private id: number;
  private isRunning: boolean = false;
  private taskQueue: Task[] = [];
  private currentTask: Task | null = null;
  private onTaskComplete: ((result: any) => void) | null = null;

  constructor(id: number) {
    this.id = id;
  }

  async start(): Promise<void> {
    this.isRunning = true;
    console.log(`Worker thread ${this.id} started`);
    
    while (this.isRunning) {
      if (this.taskQueue.length > 0) {
        this.currentTask = this.taskQueue.shift()!;
        await this.executeTask(this.currentTask);
        this.currentTask = null;
      } else {
        await this.sleep(10); // 10ms idle
      }
    }
    
    console.log(`Worker thread ${this.id} stopped`);
  }

  stop(): void {
    this.isRunning = false;
  }

  submitTask(task: Task): void {
    this.taskQueue.push(task);
  }

  private async executeTask(task: Task): Promise<void> {
    try {
      console.log(`Worker ${this.id} executing task: ${task.name}`);
      const result = await task.execute();
      
      if (this.onTaskComplete) {
        this.onTaskComplete(result);
      }
      
      console.log(`Worker ${this.id} completed task: ${task.name}`);
    } catch (error) {
      console.error(`Worker ${this.id} error in task ${task.name}:`, error);
    }
  }

  private async sleep(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  getStatus(): WorkerStatus {
    return {
      id: this.id,
      isRunning: this.isRunning,
      queueLength: this.taskQueue.length,
      currentTask: this.currentTask?.name || null
    };
  }
}

interface Task {
  name: string;
  execute(): Promise<any>;
}

interface WorkerStatus {
  id: number;
  isRunning: boolean;
  queueLength: number;
  currentTask: string | null;
}

// Thread pool
class ThreadPool {
  private workers: WorkerThread[] = [];
  private taskQueue: Task[] = [];
  private isRunning: boolean = false;
  private completedTasks: number = 0;
  private failedTasks: number = 0;

  constructor(size: number) {
    for (let i = 0; i < size; i++) {
      const worker = new WorkerThread(i);
      this.workers.push(worker);
    }
  }

  async start(): Promise<void> {
    this.isRunning = true;
    console.log(`Starting thread pool with ${this.workers.length} workers`);
    
    // Start all workers
    const workerPromises = this.workers.map(worker => worker.start());
    await Promise.all(workerPromises);
  }

  stop(): void {
    this.isRunning = false;
    this.workers.forEach(worker => worker.stop());
    console.log('Thread pool stopped');
  }

  submitTask(task: Task): void {
    if (!this.isRunning) {
      throw new Error('Thread pool is not running');
    }
    
    // Find worker with shortest queue
    const worker = this.workers.reduce((min, current) => 
      current.getStatus().queueLength < min.getStatus().queueLength ? current : min
    );
    
    worker.submitTask(task);
  }

  getStats(): ThreadPoolStats {
    const workerStats = this.workers.map(worker => worker.getStatus());
    
    return {
      totalWorkers: this.workers.length,
      activeWorkers: workerStats.filter(w => w.isRunning).length,
      totalQueueLength: workerStats.reduce((sum, w) => sum + w.queueLength, 0),
      completedTasks: this.completedTasks,
      failedTasks: this.failedTasks,
      workerStats
    };
  }
}

interface ThreadPoolStats {
  totalWorkers: number;
  activeWorkers: number;
  totalQueueLength: number;
  completedTasks: number;
  failedTasks: number;
  workerStats: WorkerStatus[];
}

// Concurrent data structures
class ConcurrentQueue<T> {
  private queue: T[] = [];
  private mutex = new Mutex();

  async enqueue(item: T): Promise<void> {
    await this.mutex.lock();
    try {
      this.queue.push(item);
    } finally {
      this.mutex.unlock();
    }
  }

  async dequeue(): Promise<T | null> {
    await this.mutex.lock();
    try {
      return this.queue.shift() || null;
    } finally {
      this.mutex.unlock();
    }
  }

  async size(): Promise<number> {
    await this.mutex.lock();
    try {
      return this.queue.length;
    } finally {
      this.mutex.unlock();
    }
  }

  async isEmpty(): Promise<boolean> {
    return (await this.size()) === 0;
  }
}

class ConcurrentMap<K, V> {
  private map = new Map<K, V>();
  private mutex = new Mutex();

  async set(key: K, value: V): Promise<void> {
    await this.mutex.lock();
    try {
      this.map.set(key, value);
    } finally {
      this.mutex.unlock();
    }
  }

  async get(key: K): Promise<V | undefined> {
    await this.mutex.lock();
    try {
      return this.map.get(key);
    } finally {
      this.mutex.unlock();
    }
  }

  async has(key: K): Promise<boolean> {
    await this.mutex.lock();
    try {
      return this.map.has(key);
    } finally {
      this.mutex.unlock();
    }
  }

  async delete(key: K): Promise<boolean> {
    await this.mutex.lock();
    try {
      return this.map.delete(key);
    } finally {
      this.mutex.unlock();
    }
  }

  async size(): Promise<number> {
    await this.mutex.lock();
    try {
      return this.map.size;
    } finally {
      this.mutex.unlock();
    }
  }

  async clear(): Promise<void> {
    await this.mutex.lock();
    try {
      this.map.clear();
    } finally {
      this.mutex.unlock();
    }
  }
}

// Mutex implementation
class Mutex {
  private locked: boolean = false;
  private waitingQueue: (() => void)[] = [];

  async lock(): Promise<void> {
    return new Promise<void>((resolve) => {
      if (!this.locked) {
        this.locked = true;
        resolve();
      } else {
        this.waitingQueue.push(resolve);
      }
    });
  }

  unlock(): void {
    if (this.waitingQueue.length > 0) {
      const next = this.waitingQueue.shift()!;
      next();
    } else {
      this.locked = false;
    }
  }

  isLocked(): boolean {
    return this.locked;
  }
}

// Semaphore implementation
class Semaphore {
  private count: number;
  private waitingQueue: (() => void)[] = [];

  constructor(initialCount: number) {
    this.count = initialCount;
  }

  async acquire(): Promise<void> {
    return new Promise<void>((resolve) => {
      if (this.count > 0) {
        this.count--;
        resolve();
      } else {
        this.waitingQueue.push(resolve);
      }
    });
  }

  release(): void {
    if (this.waitingQueue.length > 0) {
      const next = this.waitingQueue.shift()!;
      next();
    } else {
      this.count++;
    }
  }

  getCount(): number {
    return this.count;
  }

  getWaitingCount(): number {
    return this.waitingQueue.length;
  }
}

// Barrier implementation
class Barrier {
  private count: number;
  private waiting: number = 0;
  private generation: number = 0;
  private waitingQueue: (() => void)[] = [];

  constructor(count: number) {
    this.count = count;
  }

  async await(): Promise<void> {
    const myGeneration = this.generation;
    this.waiting++;

    if (this.waiting === this.count) {
      this.waiting = 0;
      this.generation++;
      
      // Release all waiting threads
      const toRelease = this.waitingQueue.splice(0);
      toRelease.forEach(resolve => resolve());
    } else {
      return new Promise<void>((resolve) => {
        this.waitingQueue.push(resolve);
      });
    }
  }

  getWaitingCount(): number {
    return this.waiting;
  }

  getGeneration(): number {
    return this.generation;
  }
}

// Producer-Consumer pattern
class ProducerConsumer<T> {
  private queue: ConcurrentQueue<T>;
  private producers: number = 0;
  private consumers: number = 0;
  private isRunning: boolean = false;

  constructor(queueSize: number = 100) {
    this.queue = new ConcurrentQueue<T>();
  }

  async startProducer(producerId: number, produce: () => T): Promise<void> {
    this.producers++;
    this.isRunning = true;
    
    console.log(`Producer ${producerId} started`);
    
    while (this.isRunning) {
      try {
        const item = produce();
        await this.queue.enqueue(item);
        console.log(`Producer ${producerId} produced: ${item}`);
        
        // Simulate production time
        await this.sleep(Math.random() * 1000);
      } catch (error) {
        console.error(`Producer ${producerId} error:`, error);
      }
    }
    
    console.log(`Producer ${producerId} stopped`);
  }

  async startConsumer(consumerId: number, consume: (item: T) => void): Promise<void> {
    this.consumers++;
    
    console.log(`Consumer ${consumerId} started`);
    
    while (this.isRunning) {
      try {
        const item = await this.queue.dequeue();
        if (item !== null) {
          consume(item);
          console.log(`Consumer ${consumerId} consumed: ${item}`);
        }
        
        // Simulate consumption time
        await this.sleep(Math.random() * 500);
      } catch (error) {
        console.error(`Consumer ${consumerId} error:`, error);
      }
    }
    
    console.log(`Consumer ${consumerId} stopped`);
  }

  stop(): void {
    this.isRunning = false;
    console.log('Producer-Consumer system stopped');
  }

  async getQueueSize(): Promise<number> {
    return await this.queue.size();
  }

  private async sleep(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms));
  }
}

// Parallel processing
class ParallelProcessor {
  private threadPool: ThreadPool;

  constructor(workerCount: number = 4) {
    this.threadPool = new ThreadPool(workerCount);
  }

  async start(): Promise<void> {
    await this.threadPool.start();
  }

  async stop(): void {
    this.threadPool.stop();
  }

  async processArray<T, R>(
    array: T[],
    processor: (item: T, index: number) => Promise<R>,
    batchSize: number = 10
  ): Promise<R[]> {
    const results: R[] = new Array(array.length);
    const batches: number[][] = [];
    
    // Create batches
    for (let i = 0; i < array.length; i += batchSize) {
      const batch = [];
      for (let j = i; j < Math.min(i + batchSize, array.length); j++) {
        batch.push(j);
      }
      batches.push(batch);
    }
    
    // Process batches in parallel
    const batchPromises = batches.map(async (batch, batchIndex) => {
      const batchResults: R[] = [];
      
      for (const index of batch) {
        const result = await processor(array[index], index);
        batchResults.push(result);
        results[index] = result;
      }
      
      console.log(`Batch ${batchIndex} completed: ${batchResults.length} items`);
      return batchResults;
    });
    
    await Promise.all(batchPromises);
    return results;
  }

  async mapReduce<T, K, V, R>(
    array: T[],
    mapper: (item: T) => Promise<Map<K, V>>,
    reducer: (key: K, values: V[]) => Promise<R>,
    batchSize: number = 10
  ): Promise<Map<K, R>> {
    // Map phase
    const mapResults = await this.processArray(array, mapper, batchSize);
    
    // Collect all key-value pairs
    const keyValuePairs = new Map<K, V[]>();
    
    for (const mapResult of mapResults) {
      for (const [key, value] of mapResult) {
        if (!keyValuePairs.has(key)) {
          keyValuePairs.set(key, []);
        }
        keyValuePairs.get(key)!.push(value);
      }
    }
    
    // Reduce phase
    const reduceResults = new Map<K, R>();
    
    for (const [key, values] of keyValuePairs) {
      const result = await reducer(key, values);
      reduceResults.set(key, result);
    }
    
    return reduceResults;
  }
}

// Usage examples
async function demonstrateConcurrentSystem(): Promise<void> {
  console.log("=== Concurrent System Demo ===\n");

  // 1. Thread pool
  console.log("1. Thread Pool:");
  const threadPool = new ThreadPool(3);
  
  // Start thread pool
  threadPool.start().catch(console.error);
  
  // Submit some tasks
  for (let i = 0; i < 5; i++) {
    threadPool.submitTask({
      name: `Task ${i}`,
      execute: async () => {
        await new Promise(resolve => setTimeout(resolve, Math.random() * 1000));
        return `Result ${i}`;
      }
    });
  }
  
  // Wait a bit and check stats
  await new Promise(resolve => setTimeout(resolve, 2000));
  const stats = threadPool.getStats();
  console.log('Thread pool stats:', stats);
  
  threadPool.stop();

  // 2. Concurrent data structures
  console.log("\n2. Concurrent Data Structures:");
  const concurrentQueue = new ConcurrentQueue<string>();
  const concurrentMap = new ConcurrentMap<string, number>();
  
  // Test concurrent queue
  await concurrentQueue.enqueue('item1');
  await concurrentQueue.enqueue('item2');
  await concurrentQueue.enqueue('item3');
  
  console.log('Queue size:', await concurrentQueue.size());
  console.log('Dequeued:', await concurrentQueue.dequeue());
  console.log('Queue size after dequeue:', await concurrentQueue.size());
  
  // Test concurrent map
  await concurrentMap.set('key1', 100);
  await concurrentMap.set('key2', 200);
  
  console.log('Map has key1:', await concurrentMap.has('key1'));
  console.log('Map get key2:', await concurrentMap.get('key2'));
  console.log('Map size:', await concurrentMap.size());

  // 3. Semaphore
  console.log("\n3. Semaphore:");
  const semaphore = new Semaphore(2);
  
  const semaphoreTask = async (id: number) => {
    console.log(`Task ${id} waiting for semaphore`);
    await semaphore.acquire();
    console.log(`Task ${id} acquired semaphore`);
    
    await new Promise(resolve => setTimeout(resolve, 1000));
    
    semaphore.release();
    console.log(`Task ${id} released semaphore`);
  };
  
  // Run multiple tasks concurrently
  const semaphoreTasks = [
    semaphoreTask(1),
    semaphoreTask(2),
    semaphoreTask(3),
    semaphoreTask(4)
  ];
  
  await Promise.all(semaphoreTasks);

  // 4. Barrier
  console.log("\n4. Barrier:");
  const barrier = new Barrier(3);
  
  const barrierTask = async (id: number) => {
    console.log(`Task ${id} reached barrier`);
    await barrier.await();
    console.log(`Task ${id} passed barrier`);
  };
  
  // Run tasks that will synchronize at barrier
  const barrierTasks = [
    barrierTask(1),
    barrierTask(2),
    barrierTask(3)
  ];
  
  await Promise.all(barrierTasks);

  // 5. Producer-Consumer
  console.log("\n5. Producer-Consumer:");
  const producerConsumer = new ProducerConsumer<number>();
  
  // Start producers
  const producer1 = producerConsumer.startProducer(1, () => Math.floor(Math.random() * 100));
  const producer2 = producerConsumer.startProducer(2, () => Math.floor(Math.random() * 100));
  
  // Start consumers
  const consumer1 = producerConsumer.startConsumer(1, (item) => {
    console.log(`Consumer 1 processed: ${item * 2}`);
  });
  const consumer2 = producerConsumer.startConsumer(2, (item) => {
    console.log(`Consumer 2 processed: ${item * 3}`);
  });
  
  // Let it run for a bit
  await new Promise(resolve => setTimeout(resolve, 3000));
  
  console.log('Queue size:', await producerConsumer.getQueueSize());
  producerConsumer.stop();

  // 6. Parallel processing
  console.log("\n6. Parallel Processing:");
  const parallelProcessor = new ParallelProcessor(2);
  await parallelProcessor.start();
  
  // Process array in parallel
  const numbers = Array.from({ length: 20 }, (_, i) => i + 1);
  
  const results = await parallelProcessor.processArray(
    numbers,
    async (num, index) => {
      await new Promise(resolve => setTimeout(resolve, 100));
      return num * num;
    },
    5
  );
  
  console.log('Parallel processing results:', results.slice(0, 10));
  
  // Map-reduce example
  const words = ['hello', 'world', 'hello', 'typescript', 'world', 'hello'];
  
  const wordCounts = await parallelProcessor.mapReduce(
    words,
    async (word) => {
      const map = new Map<string, number>();
      map.set(word, 1);
      return map;
    },
    async (word, counts) => {
      return counts.reduce((sum, count) => sum + count, 0);
    }
  );
  
  console.log('Word counts:', Object.fromEntries(wordCounts));
  
  await parallelProcessor.stop();
}

// This example demonstrates building concurrent and parallel systems with TypeScript