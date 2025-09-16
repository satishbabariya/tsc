// System tests for end-to-end generics functionality
interface ApiResponse<T> {
    data: T;
    status: number;
    message: string;
}

interface PaginatedResponse<T> {
    data: T[];
    total: number;
    page: number;
    limit: number;
}

// Generic API client
class ApiClient {
    async get<T>(url: string): Promise<ApiResponse<T>> {
        // Simulate API call
        return new Promise(resolve => {
            setTimeout(() => {
                resolve({
                    data: {} as T,
                    status: 200,
                    message: "Success"
                });
            }, 100);
        });
    }

    async post<T, R>(url: string, data: T): Promise<ApiResponse<R>> {
        // Simulate API call
        return new Promise(resolve => {
            setTimeout(() => {
                resolve({
                    data: {} as R,
                    status: 201,
                    message: "Created"
                });
            }, 100);
        });
    }

    async getPaginated<T>(url: string, page: number = 1, limit: number = 10): Promise<PaginatedResponse<T>> {
        // Simulate API call
        return new Promise(resolve => {
            setTimeout(() => {
                resolve({
                    data: [] as T[],
                    total: 0,
                    page,
                    limit
                });
            }, 100);
        });
    }
}

// Generic data processor
class DataProcessor<T> {
    constructor(private transformer: (item: T) => T) {
    }

    process(items: T[]): T[] {
        return items.map(this.transformer);
    }

    filter(items: T[], predicate: (item: T) => boolean): T[] {
        return items.filter(predicate);
    }

    reduce<R>(items: T[], reducer: (acc: R, item: T) => R, initial: R): R {
        return items.reduce(reducer, initial);
    }
}

// Generic cache
class Cache<K, V> {
    private cache = new Map<K, V>();
    private ttl = new Map<K, number>();
    private defaultTtl = 60000; // 1 minute

    set(key: K, value: V, ttl?: number): void {
        this.cache.set(key, value);
        this.ttl.set(key, Date.now() + (ttl || this.defaultTtl));
    }

    get(key: K): V | undefined {
        const expiry = this.ttl.get(key);
        if (expiry && Date.now() > expiry) {
            this.cache.delete(key);
            this.ttl.delete(key);
            return undefined;
        }
        return this.cache.get(key);
    }

    has(key: K): boolean {
        return this.get(key) !== undefined;
    }

    delete(key: K): boolean {
        this.ttl.delete(key);
        return this.cache.delete(key);
    }

    clear(): void {
        this.cache.clear();
        this.ttl.clear();
    }
}

// Generic event system
class EventEmitter<T extends Record<string, any[]>> {
    private listeners = new Map<keyof T, Array<(...args: any[]) => void>>();

    on<K extends keyof T>(event: K, listener: (...args: T[K]) => void): void {
        if (!this.listeners.has(event)) {
            this.listeners.set(event, []);
        }
        this.listeners.get(event)!.push(listener);
    }

    emit<K extends keyof T>(event: K, ...args: T[K]): void {
        const eventListeners = this.listeners.get(event);
        if (eventListeners) {
            eventListeners.forEach(listener => listener(...args));
        }
    }

    off<K extends keyof T>(event: K, listener: (...args: T[K]) => void): void {
        const eventListeners = this.listeners.get(event);
        if (eventListeners) {
            const index = eventListeners.indexOf(listener);
            if (index > -1) {
                eventListeners.splice(index, 1);
            }
        }
    }
}

// Test system functionality
const apiClient = new ApiClient();
const dataProcessor = new DataProcessor<number>(x => x * 2);
const cache = new Cache<string, number>();
const eventEmitter = new EventEmitter<{
    'user:created': [User];
    'user:updated': [User];
    'user:deleted': [number];
}>();

// Test API client
apiClient.get<User>('/api/users/1').then(response => {
    console.log('API response:', response);
});

// Test data processor
const numbers = [1, 2, 3, 4, 5];
const processed = dataProcessor.process(numbers);
const filtered = dataProcessor.filter(processed, x => x > 4);
const sum = dataProcessor.reduce(numbers, (acc, x) => acc + x, 0);

console.log('Processed:', processed);
console.log('Filtered:', filtered);
console.log('Sum:', sum);

// Test cache
cache.set('key1', 42);
cache.set('key2', 100, 5000); // 5 second TTL
console.log('Cache get:', cache.get('key1'));
console.log('Cache has:', cache.has('key2'));

// Test event emitter
eventEmitter.on('user:created', (user) => {
    console.log('User created:', user);
});

eventEmitter.on('user:updated', (user) => {
    console.log('User updated:', user);
});

eventEmitter.emit('user:created', {id: 1, name: 'Alice', email: 'alice@example.com'});
eventEmitter.emit('user:updated', {id: 1, name: 'Alice Updated', email: 'alice@example.com'});
