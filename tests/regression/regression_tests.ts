// Regression tests for error handling
// These tests ensure existing functionality remains intact

// Test 1: Basic TypeScript compilation (without error handling)
function basicTypeScriptTest(): void {
    interface User {
        id: string;
        name: string;
        email: string;
    }

    class UserService {
        private users: User[] = [];

        addUser(user: User): void {
            this.users.push(user);
        }

        getUser(id: string): User | undefined {
            return this.users.find(user => user.id === id);
        }

        getAllUsers(): User[] {
            return [...this.users];
        }
    }

    const userService = new UserService();
    userService.addUser({id: "1", name: "Alice", email: "alice@example.com"});
    userService.addUser({id: "2", name: "Bob", email: "bob@example.com"});

    const user = userService.getUser("1");
    console.log("User found:", user);
}

// Test 2: Generic types (without error handling)
function genericTypesTest(): void {
    interface Repository<T> {
        findById(id: string): T | null;

        save(entity: T): void;

        findAll(): T[];
    }

    class InMemoryRepository<T> implements Repository<T> {
        private entities: T[] = [];

        findById(id: string): T | null {
            return this.entities.find((entity: any) => entity.id === id) || null;
        }

        save(entity: T): void {
            this.entities.push(entity);
        }

        findAll(): T[] {
            return [...this.entities];
        }
    }

    const userRepo = new InMemoryRepository<User>();
    userRepo.save({id: "1", name: "Alice", email: "alice@example.com"});

    const foundUser = userRepo.findById("1");
    console.log("Found user:", foundUser);
}

// Test 3: Async/await (without error handling)
async function asyncAwaitTest(): Promise<void> {
    async function fetchData(url: string): Promise<any> {
        // Simulate async operation
        await new Promise(resolve => setTimeout(resolve, 100));
        return {data: "success", url};
    }

    const result = await fetchData("https://api.example.com/data");
    console.log("Async result:", result);
}

// Test 4: Class inheritance (without error handling)
abstract class BaseEntity {
    constructor(public id: string) {
    }

    abstract validate(): boolean;
}

class UserEntity extends BaseEntity {
    constructor(id: string, public name: string, public email: string) {
        super(id);
    }

    validate(): boolean {
        return !!(this.name && this.email);
    }
}

// Test 5: Function overloading (without error handling)
function processData(data: string): string;
function processData(data: number): number;
function processData(data: boolean): boolean;
function processData(data: any): any {
    return data;
}

// Test 6: Module system (without error handling)
export class ModuleTest {
    public test(): void {
        console.log("Module test successful");
    }
}

// Run regression tests
console.log("=== Regression Tests ===");

basicTypeScriptTest();
genericTypesTest();
asyncAwaitTest();

const userEntity = new UserEntity("1", "Alice", "alice@example.com");
console.log("User entity valid:", userEntity.validate());

console.log("Process string:", processData("test"));
console.log("Process number:", processData(123));
console.log("Process boolean:", processData(true));

const moduleTest = new ModuleTest();
moduleTest.test();
