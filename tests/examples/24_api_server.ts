// API Server Example
// Demonstrates building a complete REST API server with TypeScript

import {HTTPRequest, HTTPResponse, HTTPServer} from './23_systems_programming';

// User model
interface User {
    id: number;
    name: string;
    email: string;
    createdAt: Date;
}

// Database simulation
class UserRepository {
    private users = new Map<number, User>();
    private nextId = 1;

    async create(userData: Omit<User, 'id' | 'createdAt'>): Promise<User> {
        const user: User = {
            id: this.nextId++,
            ...userData,
            createdAt: new Date()
        };
        this.users.set(user.id, user);
        return user;
    }

    async findById(id: number): Promise<User | undefined> {
        return this.users.get(id);
    }

    async findAll(): Promise<User[]> {
        return Array.from(this.users.values());
    }

    async update(id: number, updates: Partial<User>): Promise<User | undefined> {
        const user = this.users.get(id);
        if (user) {
            const updated = {...user, ...updates};
            this.users.set(id, updated);
            return updated;
        }
        return undefined;
    }

    async delete(id: number): Promise<boolean> {
        return this.users.delete(id);
    }
}

// API Server
class APIServer {
    private server: HTTPServer;
    private userRepo: UserRepository;

    constructor() {
        this.server = new HTTPServer();
        this.userRepo = new UserRepository();
        this.setupRoutes();
        this.setupMiddleware();
    }

    async start(port: number = 3000): Promise<void> {
        _print(`Starting API server on port ${port}`);
        this.server.listen(port, () => {
            _print(`API server is running on http://localhost:${port}`);
            _print('Available endpoints:');
            _print('  GET    /api/users     - List all users');
            _print('  GET    /api/users/:id - Get user by ID');
            _print('  POST   /api/users     - Create new user');
            _print('  PUT    /api/users/:id - Update user');
            _print('  DELETE /api/users/:id - Delete user');
            _print('  GET    /health        - Health check');
        });
    }

    private setupMiddleware(): void {
        // CORS middleware
        this.server.use(async (req) => {
            _print(`[${new Date().toISOString()}] ${req.method} ${req.path}`);
            return null;
        });

        // JSON parsing middleware
        this.server.use(async (req) => {
            if (req.headers['content-type'] === 'application/json') {
                try {
                    req.body = JSON.parse(req.body);
                } catch (error) {
                    return {
                        status: 400,
                        headers: {'Content-Type': 'application/json'},
                        body: JSON.stringify({error: 'Invalid JSON'})
                    };
                }
            }
            return null;
        });
    }

    private setupRoutes(): void {
        // GET /api/users - List all users
        this.server.get('/api/users', async (req) => {
            const users = await this.userRepo.findAll();
            return {
                status: 200,
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(users)
            };
        });

        // GET /api/users/:id - Get user by ID
        this.server.get('/api/users/:id', async (req) => {
            const id = parseInt(req.path.split('/').pop() || '0');
            const user = await this.userRepo.findById(id);

            if (!user) {
                return {
                    status: 404,
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({error: 'User not found'})
                };
            }

            return {
                status: 200,
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(user)
            };
        });

        // POST /api/users - Create new user
        this.server.post('/api/users', async (req) => {
            const {name, email} = req.body;

            if (!name || !email) {
                return {
                    status: 400,
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({error: 'Name and email are required'})
                };
            }

            const user = await this.userRepo.create({name, email});
            return {
                status: 201,
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(user)
            };
        });

        // PUT /api/users/:id - Update user
        this.server.put('/api/users/:id', async (req) => {
            const id = parseInt(req.path.split('/').pop() || '0');
            const updates = req.body;

            const user = await this.userRepo.update(id, updates);
            if (!user) {
                return {
                    status: 404,
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({error: 'User not found'})
                };
            }

            return {
                status: 200,
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify(user)
            };
        });

        // DELETE /api/users/:id - Delete user
        this.server.delete('/api/users/:id', async (req) => {
            const id = parseInt(req.path.split('/').pop() || '0');
            const deleted = await this.userRepo.delete(id);

            if (!deleted) {
                return {
                    status: 404,
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({error: 'User not found'})
                };
            }

            return {
                status: 204,
                headers: {},
                body: ''
            };
        });

        // Health check endpoint
        this.server.get('/health', async (req) => {
            return {
                status: 200,
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({status: 'healthy', timestamp: new Date().toISOString()})
            };
        });
    }
}

// CLI Application
class APICLI {
    private server: APIServer;

    constructor() {
        this.server = new APIServer();
    }

    async run(args: string[]): Promise<void> {
        const command = args[0];

        switch (command) {
            case 'start':
                const port = parseInt(args[1]) || 3000;
                await this.server.start(port);
                break;

            case 'help':
                this.showHelp();
                break;

            default:
                _print('Unknown command. Use "help" for available commands.');
                break;
        }
    }

    private showHelp(): void {
        _print('API Server CLI');
        _print('Commands:');
        _print('  start [port]  - Start the API server (default port: 3000)');
        _print('  help          - Show this help message');
    }
}

// Main execution
async function main(): Promise<void> {
    const cli = new APICLI();
    const args = process.argv.slice(2);
    await cli.run(args);
}

// Export for use in other modules
export {APIServer, APICLI, User, UserRepository};

// Run if this is the main module
if (require.main === module) {
    main().catch(_print);
}