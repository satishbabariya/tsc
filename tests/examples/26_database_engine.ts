// Database Engine Example
// Demonstrates building a simple database engine with TypeScript

// Storage engine
class StorageEngine {
    private pages = new Map<number, Page>();
    private nextPageId = 1;
    private pageSize = 4096; // 4KB pages

    createPage(): Page {
        const page = new Page(this.nextPageId++, this.pageSize);
        this.pages.set(page.id, page);
        return page;
    }

    getPage(id: number): Page | undefined {
        return this.pages.get(id);
    }

    writePage(page: Page): void {
        this.pages.set(page.id, page);
    }

    deletePage(id: number): void {
        this.pages.delete(id);
    }

    getStats(): StorageStats {
        return {
            totalPages: this.pages.size,
            totalSize: this.pages.size * this.pageSize,
            usedPages: Array.from(this.pages.values()).filter(p => !p.isEmpty()).length
        };
    }
}

class Page {
    constructor(
        public id: number,
        public size: number,
        private data: Uint8Array = new Uint8Array(size),
        private usedBytes: number = 0
    ) {
    }

    write(offset: number, data: Uint8Array): boolean {
        if (offset + data.length > this.size) {
            return false;
        }

        this.data.set(data, offset);
        this.usedBytes = Math.max(this.usedBytes, offset + data.length);
        return true;
    }

    read(offset: number, length: number): Uint8Array | null {
        if (offset + length > this.usedBytes) {
            return null;
        }

        return this.data.slice(offset, offset + length);
    }

    isEmpty(): boolean {
        return this.usedBytes === 0;
    }

    getUsedBytes(): number {
        return this.usedBytes;
    }

    getFreeBytes(): number {
        return this.size - this.usedBytes;
    }
}

interface StorageStats {
    totalPages: number;
    totalSize: number;
    usedPages: number;
}

// Index structures
class BTreeIndex {
    private root: BTreeNode | null = null;
    private order: number;

    constructor(order: number = 3) {
        this.order = order;
    }

    insert(key: string, value: any): void {
        if (this.root === null) {
            this.root = new BTreeNode(this.order, true);
            this.root.keys[0] = key;
            this.root.values[0] = value;
            this.root.keyCount = 1;
        } else {
            if (this.root.keyCount === 2 * this.order - 1) {
                const newRoot = new BTreeNode(this.order, false);
                newRoot.children[0] = this.root;
                this.splitChild(newRoot, 0);
                this.root = newRoot;
            }
            this.insertNonFull(this.root, key, value);
        }
    }

    search(key: string): any | null {
        return this.searchNode(this.root, key);
    }

    private insertNonFull(node: BTreeNode, key: string, value: any): void {
        let i = node.keyCount - 1;

        if (node.isLeaf) {
            while (i >= 0 && key < node.keys[i]) {
                node.keys[i + 1] = node.keys[i];
                node.values[i + 1] = node.values[i];
                i--;
            }
            node.keys[i + 1] = key;
            node.values[i + 1] = value;
            node.keyCount++;
        } else {
            while (i >= 0 && key < node.keys[i]) {
                i--;
            }
            i++;

            if (node.children[i]!.keyCount === 2 * this.order - 1) {
                this.splitChild(node, i);
                if (key > node.keys[i]) {
                    i++;
                }
            }

            this.insertNonFull(node.children[i]!, key, value);
        }
    }

    private splitChild(parent: BTreeNode, index: number): void {
        const child = parent.children[index]!;
        const newChild = new BTreeNode(this.order, child.isLeaf);

        for (let j = 0; j < this.order - 1; j++) {
            newChild.keys[j] = child.keys[j + this.order];
            newChild.values[j] = child.values[j + this.order];
        }

        if (!child.isLeaf) {
            for (let j = 0; j < this.order; j++) {
                newChild.children[j] = child.children[j + this.order];
            }
        }

        child.keyCount = this.order - 1;

        for (let j = parent.keyCount; j >= index + 1; j--) {
            parent.children[j + 1] = parent.children[j];
        }

        parent.children[index + 1] = newChild;

        for (let j = parent.keyCount - 1; j >= index; j--) {
            parent.keys[j + 1] = parent.keys[j];
            parent.values[j + 1] = parent.values[j];
        }

        parent.keys[index] = child.keys[this.order - 1];
        parent.values[index] = child.values[this.order - 1];
        parent.keyCount++;
    }

    private searchNode(node: BTreeNode | null, key: string): any | null {
        if (node === null) {
            return null;
        }

        let i = 0;
        while (i < node.keyCount && key > node.keys[i]) {
            i++;
        }

        if (i < node.keyCount && key === node.keys[i]) {
            return node.values[i];
        }

        if (node.isLeaf) {
            return null;
        }

        return this.searchNode(node.children[i], key);
    }
}

class BTreeNode {
    public keys: (string | null)[];
    public values: (any | null)[];
    public children: (BTreeNode | null)[];
    public keyCount: number;
    public isLeaf: boolean;

    constructor(order: number, isLeaf: boolean) {
        this.keys = new Array(2 * order - 1).fill(null);
        this.values = new Array(2 * order - 1).fill(null);
        this.children = new Array(2 * order).fill(null);
        this.keyCount = 0;
        this.isLeaf = isLeaf;
    }
}

// Query engine
class QueryEngine {
    private storage: StorageEngine;
    private indexes = new Map<string, BTreeIndex>();

    constructor(storage: StorageEngine) {
        this.storage = storage;
    }

    createIndex(name: string): void {
        this.indexes.set(name, new BTreeIndex());
    }

    insertIntoIndex(indexName: string, key: string, value: any): void {
        const index = this.indexes.get(indexName);
        if (index) {
            index.insert(key, value);
        }
    }

    searchIndex(indexName: string, key: string): any | null {
        const index = this.indexes.get(indexName);
        return index ? index.search(key) : null;
    }

    executeQuery(query: string): QueryResult {
        const tokens = this.tokenize(query);
        const ast = this.parse(tokens);
        return this.execute(ast);
    }

    private tokenize(query: string): Token[] {
        const tokens: Token[] = [];
        const words = query.trim().split(/\s+/);

        for (const word of words) {
            const upperWord = word.toUpperCase();
            switch (upperWord) {
                case 'SELECT':
                    tokens.push({type: 'SELECT', value: word});
                    break;
                case 'FROM':
                    tokens.push({type: 'FROM', value: word});
                    break;
                case 'WHERE':
                    tokens.push({type: 'WHERE', value: word});
                    break;
                case 'INSERT':
                    tokens.push({type: 'INSERT', value: word});
                    break;
                case 'INTO':
                    tokens.push({type: 'INTO', value: word});
                    break;
                case 'VALUES':
                    tokens.push({type: 'VALUES', value: word});
                    break;
                case 'UPDATE':
                    tokens.push({type: 'UPDATE', value: word});
                    break;
                case 'DELETE':
                    tokens.push({type: 'DELETE', value: word});
                    break;
                case '=':
                    tokens.push({type: 'EQUALS', value: word});
                    break;
                case ',':
                    tokens.push({type: 'COMMA', value: word});
                    break;
                case ';':
                    tokens.push({type: 'SEMICOLON', value: word});
                    break;
                default:
                    if (word.startsWith("'") && word.endsWith("'")) {
                        tokens.push({type: 'STRING', value: word.slice(1, -1)});
                    } else if (!isNaN(Number(word))) {
                        tokens.push({type: 'NUMBER', value: word});
                    } else {
                        tokens.push({type: 'IDENTIFIER', value: word});
                    }
                    break;
            }
        }

        return tokens;
    }

    private parse(tokens: Token[]): ASTNode {
        let index = 0;

        const parseSelect = (): ASTNode => {
            index++; // skip SELECT
            const columns: string[] = [];

            while (index < tokens.length && tokens[index].type !== 'FROM') {
                if (tokens[index].type === 'IDENTIFIER') {
                    columns.push(tokens[index].value);
                }
                index++;
            }

            index++; // skip FROM
            const table = tokens[index].value;
            index++;

            let whereClause: WhereClause | null = null;
            if (index < tokens.length && tokens[index].type === 'WHERE') {
                index++; // skip WHERE
                const column = tokens[index].value;
                index++;
                index++; // skip =
                const value = tokens[index].value;
                index++;
                whereClause = {column, value};
            }

            return {
                type: 'SELECT',
                columns,
                table,
                where: whereClause
            };
        };

        const parseInsert = (): ASTNode => {
            index++; // skip INSERT
            index++; // skip INTO
            const table = tokens[index].value;
            index++;

            const columns: string[] = [];
            if (tokens[index].type === '(') {
                index++; // skip (
                while (tokens[index].type !== ')') {
                    if (tokens[index].type === 'IDENTIFIER') {
                        columns.push(tokens[index].value);
                    }
                    index++;
                }
                index++; // skip )
            }

            index++; // skip VALUES
            const values: string[] = [];
            if (tokens[index].type === '(') {
                index++; // skip (
                while (tokens[index].type !== ')') {
                    if (tokens[index].type === 'STRING' || tokens[index].type === 'NUMBER') {
                        values.push(tokens[index].value);
                    }
                    index++;
                }
                index++; // skip )
            }

            return {
                type: 'INSERT',
                table,
                columns,
                values
            };
        };

        if (tokens[0].type === 'SELECT') {
            return parseSelect();
        } else if (tokens[0].type === 'INSERT') {
            return parseInsert();
        }

        throw new Error('Unsupported query type');
    }

    private execute(ast: ASTNode): QueryResult {
        switch (ast.type) {
            case 'SELECT':
                return this.executeSelect(ast as SelectNode);
            case 'INSERT':
                return this.executeInsert(ast as InsertNode);
            default:
                throw new Error('Unsupported AST node type');
        }
    }

    private executeSelect(node: SelectNode): QueryResult {
        // Simulate table data
        const mockData = [
            {id: 1, name: 'Alice', age: 25},
            {id: 2, name: 'Bob', age: 30},
            {id: 3, name: 'Charlie', age: 35}
        ];

        let results = mockData;

        if (node.where) {
            results = results.filter(row =>
                row[node.where!.column] == node.where!.value
            );
        }

        if (node.columns.length > 0 && !node.columns.includes('*')) {
            results = results.map(row => {
                const filtered: any = {};
                for (const col of node.columns) {
                    filtered[col] = row[col];
                }
                return filtered;
            });
        }

        return {
            success: true,
            data: results,
            rowCount: results.length
        };
    }

    private executeInsert(node: InsertNode): QueryResult {
        _print(`Inserting into ${node.table}:`, node.values);
        return {
            success: true,
            data: [],
            rowCount: 1
        };
    }
}

interface Token {
    type: string;
    value: string;
}

interface ASTNode {
    type: string;
}

interface SelectNode extends ASTNode {
    type: 'SELECT';
    columns: string[];
    table: string;
    where: WhereClause | null;
}

interface InsertNode extends ASTNode {
    type: 'INSERT';
    table: string;
    columns: string[];
    values: string[];
}

interface WhereClause {
    column: string;
    value: string;
}

interface QueryResult {
    success: boolean;
    data: any[];
    rowCount: number;
}

// Transaction manager
class TransactionManager {
    private activeTransactions = new Map<string, Transaction>();
    private nextTransactionId = 1;

    beginTransaction(): string {
        const id = `tx_${this.nextTransactionId++}`;
        const transaction = new Transaction(id);
        this.activeTransactions.set(id, transaction);
        return id;
    }

    commitTransaction(id: string): boolean {
        const transaction = this.activeTransactions.get(id);
        if (!transaction) {
            return false;
        }

        transaction.commit();
        this.activeTransactions.delete(id);
        return true;
    }

    rollbackTransaction(id: string): boolean {
        const transaction = this.activeTransactions.get(id);
        if (!transaction) {
            return false;
        }

        transaction.rollback();
        this.activeTransactions.delete(id);
        return true;
    }

    getActiveTransactions(): string[] {
        return Array.from(this.activeTransactions.keys());
    }
}

class Transaction {
    private operations: TransactionOperation[] = [];
    private state: 'ACTIVE' | 'COMMITTED' | 'ROLLED_BACK' = 'ACTIVE';

    constructor(public id: string) {
    }

    addOperation(operation: TransactionOperation): void {
        if (this.state === 'ACTIVE') {
            this.operations.push(operation);
        }
    }

    commit(): void {
        if (this.state === 'ACTIVE') {
            for (const operation of this.operations) {
                operation.execute();
            }
            this.state = 'COMMITTED';
        }
    }

    rollback(): void {
        if (this.state === 'ACTIVE') {
            for (let i = this.operations.length - 1; i >= 0; i--) {
                this.operations[i].rollback();
            }
            this.state = 'ROLLED_BACK';
        }
    }

    getState(): string {
        return this.state;
    }
}

interface TransactionOperation {
    execute(): void;

    rollback(): void;
}

// Database engine
class DatabaseEngine {
    private storage: StorageEngine;
    private queryEngine: QueryEngine;
    private transactionManager: TransactionManager;

    constructor() {
        this.storage = new StorageEngine();
        this.queryEngine = new QueryEngine(this.storage);
        this.transactionManager = new TransactionManager();
        this.initialize();
    }

    executeQuery(query: string): QueryResult {
        return this.queryEngine.executeQuery(query);
    }

    beginTransaction(): string {
        return this.transactionManager.beginTransaction();
    }

    commitTransaction(id: string): boolean {
        return this.transactionManager.commitTransaction(id);
    }

    rollbackTransaction(id: string): boolean {
        return this.transactionManager.rollbackTransaction(id);
    }

    getStorageStats(): StorageStats {
        return this.storage.getStats();
    }

    getActiveTransactions(): string[] {
        return this.transactionManager.getActiveTransactions();
    }

    private initialize(): void {
        // Create default indexes
        this.queryEngine.createIndex('users_id');
        this.queryEngine.createIndex('users_name');
    }
}

// Usage examples
async function demonstrateDatabaseEngine(): Promise<void> {
    _print("=== Database Engine Demo ===\n");

    const db = new DatabaseEngine();

    // 1. Basic queries
    _print("1. Basic Queries:");
    const selectResult = db.executeQuery("SELECT * FROM users");
    _print("SELECT result:", selectResult);

    const selectWhereResult = db.executeQuery("SELECT name, age FROM users WHERE age = 30");
    _print("SELECT WHERE result:", selectWhereResult);

    // 2. Insert operations
    _print("\n2. Insert Operations:");
    const insertResult = db.executeQuery("INSERT INTO users VALUES ('David', 28)");
    _print("INSERT result:", insertResult);

    // 3. Transactions
    _print("\n3. Transactions:");
    const tx1 = db.beginTransaction();
    const tx2 = db.beginTransaction();

    _print("Active transactions:", db.getActiveTransactions());

    db.commitTransaction(tx1);
    db.rollbackTransaction(tx2);

    _print("After commit/rollback:", db.getActiveTransactions());

    // 4. Storage statistics
    _print("\n4. Storage Statistics:");
    const stats = db.getStorageStats();
    _print("Storage stats:", stats);

    // 5. Index operations
    _print("\n5. Index Operations:");
    db.queryEngine.insertIntoIndex('users_name', 'Alice', {id: 1, age: 25});
    db.queryEngine.insertIntoIndex('users_name', 'Bob', {id: 2, age: 30});

    const searchResult = db.queryEngine.searchIndex('users_name', 'Alice');
    _print("Index search result:", searchResult);
}

// This example demonstrates building a complete database engine with TypeScript