// Real-World Reference Cycle Examples

// 1. Parent-Child Relationship (Common Pattern)
class TreeNode {
    value: number;
    parent: shared_ptr<TreeNode>;  // ❌ Creates cycle!
    children: shared_ptr<TreeNode>[];
    
    constructor(value: number) {
        this.value = value;
        this.children = [];
    }
    
    addChild(child: shared_ptr<TreeNode>) {
        this.children.push(child);
        child.parent = this;  // ❌ Cycle: parent -> child -> parent
    }
}

// 2. Observer Pattern (Event System)
class EventEmitter {
    listeners: shared_ptr<EventListener>[];
    
    constructor() {
        this.listeners = [];
    }
    
    addListener(listener: shared_ptr<EventListener>) {
        this.listeners.push(listener);
        listener.emitter = this;  // ❌ Cycle: emitter -> listener -> emitter
    }
}

class EventListener {
    emitter: shared_ptr<EventEmitter>;  // ❌ Creates cycle!
    
    constructor() {}
}

// 3. Database Relationships
class User {
    id: number;
    profile: shared_ptr<UserProfile>;
    
    constructor(id: number) {
        this.id = id;
    }
}

class UserProfile {
    user: shared_ptr<User>;  // ❌ Cycle: user -> profile -> user
    
    constructor(user: shared_ptr<User>) {
        this.user = user;
        user.profile = this;  // ❌ Creates bidirectional cycle
    }
}

// 4. Cache System
class Cache {
    entries: Map<string, shared_ptr<CacheEntry>>;
    
    constructor() {
        this.entries = new Map();
    }
    
    set(key: string, entry: shared_ptr<CacheEntry>) {
        this.entries.set(key, entry);
        entry.cache = this;  // ❌ Cycle: cache -> entry -> cache
    }
}

class CacheEntry {
    key: string;
    value: any;
    cache: shared_ptr<Cache>;  // ❌ Creates cycle!
    
    constructor(key: string, value: any) {
        this.key = key;
        this.value = value;
    }
}