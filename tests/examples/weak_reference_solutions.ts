// Solution 1: Weak References - Breaking Cycles Explicitly

// ✅ FIXED: Parent-Child with Weak References
class TreeNodeFixed {
    value: number;
    parent: weak_ptr<TreeNodeFixed>;  // ✅ Weak reference breaks cycle
    children: shared_ptr<TreeNodeFixed>[];
    
    constructor(value: number) {
        this.value = value;
        this.children = [];
    }
    
    addChild(child: shared_ptr<TreeNodeFixed>) {
        this.children.push(child);
        child.parent = new weak_ptr<TreeNodeFixed>(this);  // ✅ Weak reference
    }
    
    getParent(): shared_ptr<TreeNodeFixed> | null {
        return this.parent.lock();  // ✅ Safe access via weak_ptr
    }
}

// ✅ FIXED: Observer Pattern with Weak References
class EventEmitterFixed {
    listeners: shared_ptr<EventListenerFixed>[];
    
    constructor() {
        this.listeners = [];
    }
    
    addListener(listener: shared_ptr<EventListenerFixed>) {
        this.listeners.push(listener);
        listener.emitter = new weak_ptr<EventEmitterFixed>(this);  // ✅ Weak reference
    }
    
    emit(event: string) {
        // Clean up dead listeners
        this.listeners = this.listeners.filter(listener => {
            const emitter = listener.emitter.lock();
            return emitter !== null;  // ✅ Remove dead references
        });
        
        // Notify remaining listeners
        for (let listener of this.listeners) {
            listener.handleEvent(event);
        }
    }
}

class EventListenerFixed {
    emitter: weak_ptr<EventEmitterFixed>;  // ✅ Weak reference
    
    constructor() {}
    
    handleEvent(event: string) {
        const emitter = this.emitter.lock();
        if (emitter) {
            _print(`Event received: ${event}`);
        }
    }
}

// ✅ FIXED: Database Relationships with Weak References
class UserFixed {
    id: number;
    profile: shared_ptr<UserProfileFixed>;
    
    constructor(id: number) {
        this.id = id;
    }
}

class UserProfileFixed {
    user: weak_ptr<UserFixed>;  // ✅ Weak reference breaks cycle
    
    constructor(user: shared_ptr<UserFixed>) {
        this.user = new weak_ptr<UserFixed>(user);  // ✅ Weak reference
        user.profile = new shared_ptr<UserProfileFixed>(this);
    }
    
    getUser(): shared_ptr<UserFixed> | null {
        return this.user.lock();  // ✅ Safe access
    }
}

// ✅ FIXED: Cache System with Weak References
class CacheFixed {
    entries: Map<string, shared_ptr<CacheEntryFixed>>;
    
    constructor() {
        this.entries = new Map();
    }
    
    set(key: string, entry: shared_ptr<CacheEntryFixed>) {
        this.entries.set(key, entry);
        entry.cache = new weak_ptr<CacheFixed>(this);  // ✅ Weak reference
    }
    
    cleanup() {
        // Remove entries that no longer reference this cache
        for (let [key, entry] of this.entries) {
            const cache = entry.cache.lock();
            if (!cache) {
                this.entries.delete(key);  // ✅ Cleanup dead references
            }
        }
    }
}

class CacheEntryFixed {
    key: string;
    value: any;
    cache: weak_ptr<CacheFixed>;  // ✅ Weak reference
    
    constructor(key: string, value: any) {
        this.key = key;
        this.value = value;
    }
    
    getCache(): shared_ptr<CacheFixed> | null {
        return this.cache.lock();  // ✅ Safe access
    }
}