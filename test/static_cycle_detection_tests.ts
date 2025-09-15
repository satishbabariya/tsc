// Static Cycle Detection Tests
// These tests validate the compiler's ability to detect cycles at compile time

// ✅ TEST 1: Cycle Safe Classes
@cycle_safe
class SafeParent {
    children: shared_ptr<SafeChild>[];
    
    constructor() {
        this.children = [];
    }
    
    addChild(child: shared_ptr<SafeChild>) {
        this.children.push(child);
        child.parent = new weak_ptr<SafeParent>(this);  // ✅ Weak reference
    }
}

@cycle_safe
class SafeChild {
    parent: weak_ptr<SafeParent>;  // ✅ Weak reference breaks cycle
    
    constructor() {}
    
    getParent(): shared_ptr<SafeParent> | null {
        return this.parent.lock();
    }
}

// ❌ TEST 2: Cycle Unsafe Classes (Should generate warnings)
@cycle_unsafe
class UnsafeParent {
    child: shared_ptr<UnsafeChild>;  // ❌ Strong reference
    
    constructor() {}
    
    setChild(child: shared_ptr<UnsafeChild>) {
        this.child = child;
        child.parent = new shared_ptr<UnsafeParent>(this);  // ❌ Creates cycle
    }
}

@cycle_unsafe
class UnsafeChild {
    parent: shared_ptr<UnsafeParent>;  // ❌ Strong reference creates cycle
    
    constructor() {}
}

// ✅ TEST 3: Ownership Hierarchy Pattern
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
    document: weak_ptr<Document>;  // ✅ Weak reference to parent
    
    constructor(content: string) {
        this.content = content;
    }
    
    getDocument(): shared_ptr<Document> | null {
        return this.document.lock();
    }
}

// ✅ TEST 4: Observer Pattern
@observer_pattern
class EventEmitter {
    listeners: shared_ptr<EventListener>[];
    
    constructor() {
        this.listeners = [];
    }
    
    addListener(listener: shared_ptr<EventListener>) {
        this.listeners.push(listener);
        listener.emitter = new weak_ptr<EventEmitter>(this);  // ✅ Weak reference
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
    emitter: weak_ptr<EventEmitter>;  // ✅ Weak reference
    
    constructor() {}
    
    handleEvent(event: string) {
        const emitter = this.emitter.lock();
        if (emitter) {
            _print(`Event received: ${event}`);
        }
    }
}

// ✅ TEST 5: Resource Manager Pattern
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

// ✅ TEST 6: Cache Pattern
@cache_pattern
class Cache {
    entries: Map<string, shared_ptr<CacheEntry>>;
    
    constructor() {
        this.entries = new Map();
    }
    
    set(key: string, entry: shared_ptr<CacheEntry>) {
        this.entries.set(key, entry);
        entry.cache = new weak_ptr<Cache>(this);  // ✅ Weak reference
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
    cache: weak_ptr<Cache>;  // ✅ Weak reference
    
    constructor(key: string, value: any) {
        this.key = key;
        this.value = value;
    }
    
    getCache(): shared_ptr<Cache> | null {
        return this.cache.lock();
    }
}

// ✅ TEST 7: Factory Pattern
@factory_pattern
class ObjectFactory {
    prototypes: Map<string, shared_ptr<Prototype>>;
    
    constructor() {
        this.prototypes = new Map();
    }
    
    registerPrototype(name: string, prototype: shared_ptr<Prototype>) {
        this.prototypes.set(name, prototype);
        prototype.factory = new weak_ptr<ObjectFactory>(this);  // ✅ Weak reference
    }
    
    createObject(name: string): shared_ptr<Object> | null {
        const prototype = this.prototypes.get(name);
        return prototype ? prototype.createInstance() : null;
    }
}

@factory_pattern
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

@factory_pattern
class Object {
    name: string;
    
    constructor(name: string) {
        this.name = name;
    }
}

// ❌ TEST 8: Complex Cycle (Should be detected)
class NodeA {
    nodeB: shared_ptr<NodeB>;  // ❌ Strong reference
}

class NodeB {
    nodeC: shared_ptr<NodeC>;  // ❌ Strong reference
}

class NodeC {
    nodeA: shared_ptr<NodeA>;  // ❌ Strong reference - creates A->B->C->A cycle
}

// ❌ TEST 9: Multiple Cycles (Should be detected)
class GraphNode {
    neighbors: shared_ptr<GraphNode>[];  // ❌ Multiple strong references
    
    constructor() {
        this.neighbors = [];
    }
    
    addNeighbor(neighbor: shared_ptr<GraphNode>) {
        this.neighbors.push(neighbor);
        neighbor.neighbors.push(new shared_ptr<GraphNode>(this));  // ❌ Bidirectional cycle
    }
}

// ✅ TEST 10: Correctly Broken Cycle
class TreeNode {
    children: shared_ptr<TreeNode>[];
    parent: weak_ptr<TreeNode>;  // ✅ Weak reference breaks cycle
    
    constructor() {
        this.children = [];
    }
    
    addChild(child: shared_ptr<TreeNode>) {
        this.children.push(child);
        child.parent = new weak_ptr<TreeNode>(this);
    }
    
    getParent(): shared_ptr<TreeNode> | null {
        return this.parent.lock();
    }
}