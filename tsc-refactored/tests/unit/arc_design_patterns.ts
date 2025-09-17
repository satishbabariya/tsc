// Solution 3: ARC Design Patterns & Best Practices

// 🎯 PATTERN 1: Ownership Hierarchy
// Rule: Parent owns children strongly, children reference parent weakly

class Document {
    pages: shared_ptr<Page>[];
    metadata: shared_ptr<DocumentMetadata>;

    constructor() {
        this.pages = [];
        this.metadata = new shared_ptr<DocumentMetadata>(new DocumentMetadata());
    }

    addPage(content: string): shared_ptr<Page> {
        let page = new shared_ptr<Page>(new Page(content));
        page.document = new weak_ptr<Document>(this);  // ✅ Weak reference to parent
        this.pages.push(page);
        return page;
    }
}

class Page {
    content: string;
    document: weak_ptr<Document>;  // ✅ Weak reference to parent

    constructor(content: string) {
        this.content = content;
    }

    getDocument(): shared_ptr<Document> | null {
        return this.document.lock();  // ✅ Safe access
    }
}

class DocumentMetadata {
    title: string;
    author: string;

    constructor() {
        this.title = "Untitled";
        this.author = "Unknown";
    }
}

// 🎯 PATTERN 2: Observer Pattern with Weak References
class EventBus {
    private subscribers: Map<string, shared_ptr<EventListener>[]>;

    constructor() {
        this.subscribers = new Map();
    }

    subscribe(event: string, listener: shared_ptr<EventListener>) {
        if (!this.subscribers.has(event)) {
            this.subscribers.set(event, []);
        }
        this.subscribers.get(event)!.push(listener);

        // ✅ Weak reference prevents cycle
        listener.eventBus = new weak_ptr<EventBus>(this);
    }

    publish(event: string, data: any) {
        const listeners = this.subscribers.get(event);
        if (!listeners) return;

        // ✅ Clean up dead listeners
        const aliveListeners = listeners.filter(listener => {
            const bus = listener.eventBus.lock();
            return bus !== null;
        });

        this.subscribers.set(event, aliveListeners);

        // Notify alive listeners
        for (let listener of aliveListeners) {
            listener.handleEvent(event, data);
        }
    }
}

class EventListener {
    eventBus: weak_ptr<EventBus>;  // ✅ Weak reference
    callback: (event: string, data: any) => void;

    constructor(callback: (event: string, data: any) => void) {
        this.callback = callback;
    }

    handleEvent(event: string, data: any) {
        this.callback(event, data);
    }
}

// 🎯 PATTERN 3: Resource Management with RAII
class ResourceManager {
    private resources: Map<string, shared_ptr<Resource>>;

    constructor() {
        this.resources = new Map();
    }

    acquireResource(id: string): shared_ptr<Resource> {
        let resource = this.resources.get(id);
        if (!resource) {
            resource = new shared_ptr<Resource>(new Resource(id));
            this.resources.set(id, resource);
        }
        return resource;
    }

    releaseResource(id: string) {
        this.resources.delete(id);
    }
}

class Resource {
    id: string;
    manager: weak_ptr<ResourceManager>;  // ✅ Weak reference

    constructor(id: string) {
        this.id = id;
    }

    setManager(manager: shared_ptr<ResourceManager>) {
        this.manager = new weak_ptr<ResourceManager>(manager);
    }

    release() {
        const mgr = this.manager.lock();
        if (mgr) {
            mgr.releaseResource(this.id);
        }
    }
}

// 🎯 PATTERN 4: Cache with Weak References
class LRUCache {
    private cache: Map<string, shared_ptr<CacheEntry>>;
    private accessOrder: string[];
    private maxSize: number;

    constructor(maxSize: number) {
        this.cache = new Map();
        this.accessOrder = [];
        this.maxSize = maxSize;
    }

    get(key: string): any | null {
        const entry = this.cache.get(key);
        if (!entry) return null;

        // Update access order
        this.accessOrder = this.accessOrder.filter(k => k !== key);
        this.accessOrder.push(key);

        return entry.value;
    }

    set(key: string, value: any) {
        // Remove oldest if at capacity
        if (this.cache.size >= this.maxSize) {
            const oldest = this.accessOrder.shift()!;
            this.cache.delete(oldest);
        }

        let entry = new shared_ptr<CacheEntry>(new CacheEntry(key, value));
        entry.cache = new weak_ptr<LRUCache>(this);  // ✅ Weak reference
        this.cache.set(key, entry);
        this.accessOrder.push(key);
    }
}

class CacheEntry {
    key: string;
    value: any;
    cache: weak_ptr<LRUCache>;  // ✅ Weak reference

    constructor(key: string, value: any) {
        this.key = key;
        this.value = value;
    }

    getCache(): shared_ptr<LRUCache> | null {
        return this.cache.lock();
    }
}

// 🎯 PATTERN 5: Factory Pattern with ARC
class ObjectFactory {
    private prototypes: Map<string, shared_ptr<Prototype>>;

    constructor() {
        this.prototypes = new Map();
    }

    registerPrototype(name: string, prototype: shared_ptr<Prototype>) {
        this.prototypes.set(name, prototype);
        prototype.factory = new weak_ptr<ObjectFactory>(this);  // ✅ Weak reference
    }

    createObject(name: string): shared_ptr<Object> | null {
        const prototype = this.prototypes.get(name);
        if (!prototype) return null;

        return prototype.createInstance();
    }
}

class Prototype {
    name: string;
    factory: weak_ptr<ObjectFactory>;  // ✅ Weak reference

    constructor(name: string) {
        this.name = name;
    }

    createInstance(): shared_ptr<Object> {
        return new shared_ptr<Object>(new Object(this.name));
    }
}

class Object {
    name: string;

    constructor(name: string) {
        this.name = name;
    }
}

// 🎯 PATTERN 6: Command Pattern with ARC
class CommandProcessor {
    private history: shared_ptr<Command>[];
    private undoStack: shared_ptr<Command>[];

    constructor() {
        this.history = [];
        this.undoStack = [];
    }

    execute(command: shared_ptr<Command>) {
        command.execute();
        this.history.push(command);
        command.processor = new weak_ptr<CommandProcessor>(this);  // ✅ Weak reference
    }

    undo() {
        if (this.history.length === 0) return;

        const command = this.history.pop()!;
        command.undo();
        this.undoStack.push(command);
    }
}

class Command {
    processor: weak_ptr<CommandProcessor>;  // ✅ Weak reference

    constructor() {
    }

    execute() {
        // Command execution logic
    }

    undo() {
        // Undo logic
    }
}