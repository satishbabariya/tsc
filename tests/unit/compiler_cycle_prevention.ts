// Solution 5: Compiler-Level Cycle Prevention
// This shows how the TSC compiler can help prevent cycles at compile time

// 🎯 COMPILER ANNOTATION: @cycle_safe
// The compiler can analyze this and warn about potential cycles

@cycle_safe
class SafeParent {
    children: shared_ptr<SafeChild>[];
    
    constructor() {
        this.children = [];
    }
    
    addChild(child: shared_ptr<SafeChild>) {
        this.children.push(child);
        // ✅ Compiler knows this is safe - parent owns child
    }
}

@cycle_safe
class SafeChild {
    parent: weak_ptr<SafeParent>;  // ✅ Compiler enforces weak reference
    
    constructor(parent: shared_ptr<SafeParent>) {
        this.parent = new weak_ptr<SafeParent>(parent);
    }
    
    getParent(): shared_ptr<SafeParent> | null {
        return this.parent.lock();
    }
}

// 🎯 COMPILER ANNOTATION: @cycle_unsafe
// The compiler will warn about potential cycles

@cycle_unsafe
class UnsafeParent {
    child: shared_ptr<UnsafeChild>;  // ⚠️ Compiler warning: potential cycle
    
    constructor() {}
    
    setChild(child: shared_ptr<UnsafeChild>) {
        this.child = child;
        child.parent = new shared_ptr<UnsafeParent>(this);  // ⚠️ Compiler error: cycle detected
    }
}

@cycle_unsafe
class UnsafeChild {
    parent: shared_ptr<UnsafeParent>;  // ⚠️ Compiler warning: potential cycle
    
    constructor() {}
}

// 🎯 COMPILER ANNOTATION: @ownership_hierarchy
// The compiler enforces ownership rules

@ownership_hierarchy
class Document {
    pages: shared_ptr<Page>[];  // ✅ Document owns pages
    
    constructor() {
        this.pages = [];
    }
    
    addPage(content: string): shared_ptr<Page> {
        let page = new shared_ptr<Page>(new Page(content));
        page.document = new weak_ptr<Document>(this);  // ✅ Page weakly references document
        this.pages.push(page);
        return page;
    }
}

@ownership_hierarchy
class Page {
    content: string;
    document: weak_ptr<Document>;  // ✅ Compiler enforces weak reference
    
    constructor(content: string) {
        this.content = content;
    }
    
    getDocument(): shared_ptr<Document> | null {
        return this.document.lock();
    }
}

// 🎯 COMPILER ANNOTATION: @observer_pattern
// The compiler enforces observer pattern rules

@observer_pattern
class EventEmitter {
    listeners: shared_ptr<EventListener>[];
    
    constructor() {
        this.listeners = [];
    }
    
    addListener(listener: shared_ptr<EventListener>) {
        this.listeners.push(listener);
        listener.emitter = new weak_ptr<EventEmitter>(this);  // ✅ Compiler enforces weak reference
    }
    
    emit(event: string) {
        // Clean up dead listeners
        this.listeners = this.listeners.filter(listener => {
            const emitter = listener.emitter.lock();
            return emitter !== null;
        });
        
        // Notify remaining listeners
        for (let listener of this.listeners) {
            listener.handleEvent(event);
        }
    }
}

@observer_pattern
class EventListener {
    emitter: weak_ptr<EventEmitter>;  // ✅ Compiler enforces weak reference
    
    constructor() {}
    
    handleEvent(event: string) {
        const emitter = this.emitter.lock();
        if (emitter) {
            _print(`Event received: ${event}`);
        }
    }
}

// 🎯 COMPILER ANNOTATION: @resource_manager
// The compiler enforces resource management rules

@resource_manager
class ResourceManager {
    resources: Map<string, shared_ptr<Resource>>;
    
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

@resource_manager
class Resource {
    id: string;
    manager: weak_ptr<ResourceManager>;  // ✅ Compiler enforces weak reference
    
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

// 🎯 COMPILER ANNOTATION: @cache_pattern
// The compiler enforces cache pattern rules

@cache_pattern
class Cache {
    entries: Map<string, shared_ptr<CacheEntry>>;
    
    constructor() {
        this.entries = new Map();
    }
    
    set(key: string, entry: shared_ptr<CacheEntry>) {
        this.entries.set(key, entry);
        entry.cache = new weak_ptr<Cache>(this);  // ✅ Compiler enforces weak reference
    }
    
    get(key: string): any | null {
        const entry = this.entries.get(key);
        return entry ? entry.value : null;
    }
}

@cache_pattern
class CacheEntry {
    key: string;
    value: any;
    cache: weak_ptr<Cache>;  // ✅ Compiler enforces weak reference
    
    constructor(key: string, value: any) {
        this.key = key;
        this.value = value;
    }
    
    getCache(): shared_ptr<Cache> | null {
        return this.cache.lock();
    }
}

// 🎯 COMPILER ANNOTATION: @factory_pattern
// The compiler enforces factory pattern rules

@factory_pattern
class ObjectFactory {
    prototypes: Map<string, shared_ptr<Prototype>>;
    
    constructor() {
        this.prototypes = new Map();
    }
    
    registerPrototype(name: string, prototype: shared_ptr<Prototype>) {
        this.prototypes.set(name, prototype);
        prototype.factory = new weak_ptr<ObjectFactory>(this);  // ✅ Compiler enforces weak reference
    }
    
    createObject(name: string): shared_ptr<Object> | null {
        const prototype = this.prototypes.get(name);
        return prototype ? prototype.createInstance() : null;
    }
}

@factory_pattern
class Prototype {
    name: string;
    factory: weak_ptr<ObjectFactory>;  // ✅ Compiler enforces weak reference
    
    constructor(name: string) {
        this.name = name;
    }
    
    createInstance(): shared_ptr<Object> {
        return new shared_ptr<Object>(new Object(this.name));
    }
}

@factory_pattern
class Object {
    name: string;
    
    constructor(name: string) {
        this.name = name;
    }
}