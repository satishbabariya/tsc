// File System Example
// Demonstrates building a file system and storage management with TypeScript

// File system node types
enum NodeType {
  FILE = 'file',
  DIRECTORY = 'directory',
  SYMLINK = 'symlink'
}

interface FileSystemNode {
  name: string;
  type: NodeType;
  size: number;
  permissions: number;
  owner: string;
  group: string;
  createdAt: Date;
  modifiedAt: Date;
  accessedAt: Date;
  parent: FileSystemNode | null;
  children: Map<string, FileSystemNode>;
}

// File system implementation
class FileSystem {
  private root: FileSystemNode;
  private currentDirectory: FileSystemNode;
  private inodeCounter = 1;

  constructor() {
    this.root = this.createNode('/', NodeType.DIRECTORY, 0, 0o755);
    this.currentDirectory = this.root;
  }

  private createNode(
    name: string,
    type: NodeType,
    size: number,
    permissions: number,
    owner: string = 'root',
    group: string = 'root'
  ): FileSystemNode {
    const now = new Date();
    return {
      name,
      type,
      size,
      permissions,
      owner,
      group,
      createdAt: now,
      modifiedAt: now,
      accessedAt: now,
      parent: null,
      children: new Map()
    };
  }

  private findNode(path: string): FileSystemNode | null {
    if (path === '/') {
      return this.root;
    }

    const parts = path.split('/').filter(part => part.length > 0);
    let current = this.currentDirectory;

    // Handle absolute paths
    if (path.startsWith('/')) {
      current = this.root;
    }

    for (const part of parts) {
      if (part === '..') {
        current = current.parent || this.root;
      } else if (part === '.') {
        // Stay in current directory
      } else {
        const child = current.children.get(part);
        if (!child) {
          return null;
        }
        current = child;
      }
    }

    return current;
  }

  private resolvePath(path: string): string {
    if (path.startsWith('/')) {
      return path;
    }
    
    const currentPath = this.getCurrentPath();
    return currentPath === '/' ? `/${path}` : `${currentPath}/${path}`;
  }

  private getCurrentPath(): string {
    const path: string[] = [];
    let current = this.currentDirectory;
    
    while (current && current !== this.root) {
      path.unshift(current.name);
      current = current.parent!;
    }
    
    return '/' + path.join('/');
  }

  // Directory operations
  mkdir(path: string, permissions: number = 0o755): void {
    const resolvedPath = this.resolvePath(path);
    const parentPath = resolvedPath.substring(0, resolvedPath.lastIndexOf('/')) || '/';
    const dirName = resolvedPath.substring(resolvedPath.lastIndexOf('/') + 1);
    
    const parent = this.findNode(parentPath);
    if (!parent) {
      throw new Error(`Parent directory not found: ${parentPath}`);
    }
    
    if (parent.type !== NodeType.DIRECTORY) {
      throw new Error(`Not a directory: ${parentPath}`);
    }
    
    if (parent.children.has(dirName)) {
      throw new Error(`Directory already exists: ${resolvedPath}`);
    }
    
    const newDir = this.createNode(dirName, NodeType.DIRECTORY, 0, permissions);
    newDir.parent = parent;
    parent.children.set(dirName, newDir);
    parent.modifiedAt = new Date();
    
    console.log(`Created directory: ${resolvedPath}`);
  }

  rmdir(path: string): void {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`Directory not found: ${resolvedPath}`);
    }
    
    if (node.type !== NodeType.DIRECTORY) {
      throw new Error(`Not a directory: ${resolvedPath}`);
    }
    
    if (node.children.size > 0) {
      throw new Error(`Directory not empty: ${resolvedPath}`);
    }
    
    if (node === this.root) {
      throw new Error('Cannot remove root directory');
    }
    
    const parent = node.parent!;
    parent.children.delete(node.name);
    parent.modifiedAt = new Date();
    
    console.log(`Removed directory: ${resolvedPath}`);
  }

  // File operations
  createFile(path: string, content: string = '', permissions: number = 0o644): void {
    const resolvedPath = this.resolvePath(path);
    const parentPath = resolvedPath.substring(0, resolvedPath.lastIndexOf('/')) || '/';
    const fileName = resolvedPath.substring(resolvedPath.lastIndexOf('/') + 1);
    
    const parent = this.findNode(parentPath);
    if (!parent) {
      throw new Error(`Parent directory not found: ${parentPath}`);
    }
    
    if (parent.type !== NodeType.DIRECTORY) {
      throw new Error(`Not a directory: ${parentPath}`);
    }
    
    if (parent.children.has(fileName)) {
      throw new Error(`File already exists: ${resolvedPath}`);
    }
    
    const newFile = this.createNode(fileName, NodeType.FILE, content.length, permissions);
    newFile.parent = parent;
    parent.children.set(fileName, newFile);
    parent.modifiedAt = new Date();
    
    console.log(`Created file: ${resolvedPath} (${content.length} bytes)`);
  }

  writeFile(path: string, content: string): void {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`File not found: ${resolvedPath}`);
    }
    
    if (node.type !== NodeType.FILE) {
      throw new Error(`Not a file: ${resolvedPath}`);
    }
    
    node.size = content.length;
    node.modifiedAt = new Date();
    node.accessedAt = new Date();
    
    console.log(`Written to file: ${resolvedPath} (${content.length} bytes)`);
  }

  readFile(path: string): string {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`File not found: ${resolvedPath}`);
    }
    
    if (node.type !== NodeType.FILE) {
      throw new Error(`Not a file: ${resolvedPath}`);
    }
    
    node.accessedAt = new Date();
    
    console.log(`Read file: ${resolvedPath} (${node.size} bytes)`);
    return `Content of ${node.name}`; // Simulated content
  }

  deleteFile(path: string): void {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`File not found: ${resolvedPath}`);
    }
    
    if (node.type !== NodeType.FILE) {
      throw new Error(`Not a file: ${resolvedPath}`);
    }
    
    const parent = node.parent!;
    parent.children.delete(node.name);
    parent.modifiedAt = new Date();
    
    console.log(`Deleted file: ${resolvedPath}`);
  }

  // Navigation operations
  cd(path: string): void {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`Directory not found: ${resolvedPath}`);
    }
    
    if (node.type !== NodeType.DIRECTORY) {
      throw new Error(`Not a directory: ${resolvedPath}`);
    }
    
    this.currentDirectory = node;
    node.accessedAt = new Date();
    
    console.log(`Changed directory to: ${resolvedPath}`);
  }

  pwd(): string {
    return this.getCurrentPath();
  }

  ls(path: string = '.'): string[] {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`Directory not found: ${resolvedPath}`);
    }
    
    if (node.type !== NodeType.DIRECTORY) {
      throw new Error(`Not a directory: ${resolvedPath}`);
    }
    
    node.accessedAt = new Date();
    
    const entries = Array.from(node.children.keys());
    console.log(`Listing directory: ${resolvedPath}`);
    return entries;
  }

  // File information
  stat(path: string): FileStats {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`File not found: ${resolvedPath}`);
    }
    
    return {
      name: node.name,
      type: node.type,
      size: node.size,
      permissions: node.permissions,
      owner: node.owner,
      group: node.group,
      createdAt: node.createdAt,
      modifiedAt: node.modifiedAt,
      accessedAt: node.accessedAt
    };
  }

  // Permissions
  chmod(path: string, permissions: number): void {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`File not found: ${resolvedPath}`);
    }
    
    node.permissions = permissions;
    node.modifiedAt = new Date();
    
    console.log(`Changed permissions for ${resolvedPath} to ${permissions.toString(8)}`);
  }

  chown(path: string, owner: string, group: string): void {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`File not found: ${resolvedPath}`);
    }
    
    node.owner = owner;
    node.group = group;
    node.modifiedAt = new Date();
    
    console.log(`Changed ownership for ${resolvedPath} to ${owner}:${group}`);
  }

  // Search operations
  find(path: string, pattern: string): string[] {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`Directory not found: ${resolvedPath}`);
    }
    
    if (node.type !== NodeType.DIRECTORY) {
      throw new Error(`Not a directory: ${resolvedPath}`);
    }
    
    const results: string[] = [];
    this.findRecursive(node, pattern, resolvedPath, results);
    
    console.log(`Found ${results.length} files matching "${pattern}" in ${resolvedPath}`);
    return results;
  }

  private findRecursive(node: FileSystemNode, pattern: string, currentPath: string, results: string[]): void {
    for (const [name, child] of node.children) {
      const childPath = currentPath === '/' ? `/${name}` : `${currentPath}/${name}`;
      
      if (child.name.includes(pattern)) {
        results.push(childPath);
      }
      
      if (child.type === NodeType.DIRECTORY) {
        this.findRecursive(child, pattern, childPath, results);
      }
    }
  }

  // Disk usage
  du(path: string = '.'): DiskUsage {
    const resolvedPath = this.resolvePath(path);
    const node = this.findNode(resolvedPath);
    
    if (!node) {
      throw new Error(`Directory not found: ${resolvedPath}`);
    }
    
    const usage = this.calculateDiskUsage(node);
    console.log(`Disk usage for ${resolvedPath}: ${usage.totalSize} bytes`);
    return usage;
  }

  private calculateDiskUsage(node: FileSystemNode): DiskUsage {
    let totalSize = node.size;
    let fileCount = node.type === NodeType.FILE ? 1 : 0;
    let dirCount = node.type === NodeType.DIRECTORY ? 1 : 0;
    
    if (node.type === NodeType.DIRECTORY) {
      for (const child of node.children.values()) {
        const childUsage = this.calculateDiskUsage(child);
        totalSize += childUsage.totalSize;
        fileCount += childUsage.fileCount;
        dirCount += childUsage.dirCount;
      }
    }
    
    return {
      totalSize,
      fileCount,
      dirCount
    };
  }

  // File system statistics
  getStats(): FileSystemStats {
    const rootUsage = this.calculateDiskUsage(this.root);
    
    return {
      totalFiles: rootUsage.fileCount,
      totalDirectories: rootUsage.dirCount,
      totalSize: rootUsage.totalSize,
      rootPath: '/',
      currentPath: this.getCurrentPath()
    };
  }
}

interface FileStats {
  name: string;
  type: NodeType;
  size: number;
  permissions: number;
  owner: string;
  group: string;
  createdAt: Date;
  modifiedAt: Date;
  accessedAt: Date;
}

interface DiskUsage {
  totalSize: number;
  fileCount: number;
  dirCount: number;
}

interface FileSystemStats {
  totalFiles: number;
  totalDirectories: number;
  totalSize: number;
  rootPath: string;
  currentPath: string;
}

// File system utilities
class FileSystemUtils {
  static formatSize(bytes: number): string {
    const units = ['B', 'KB', 'MB', 'GB', 'TB'];
    let size = bytes;
    let unitIndex = 0;
    
    while (size >= 1024 && unitIndex < units.length - 1) {
      size /= 1024;
      unitIndex++;
    }
    
    return `${size.toFixed(2)} ${units[unitIndex]}`;
  }

  static formatPermissions(permissions: number): string {
    const owner = (permissions >> 6) & 7;
    const group = (permissions >> 3) & 7;
    const other = permissions & 7;
    
    const formatOctal = (oct: number): string => {
      return (oct & 4 ? 'r' : '-') + (oct & 2 ? 'w' : '-') + (oct & 1 ? 'x' : '-');
    };
    
    return `${formatOctal(owner)}${formatOctal(group)}${formatOctal(other)}`;
  }

  static formatDate(date: Date): string {
    return date.toISOString().replace('T', ' ').substring(0, 19);
  }
}

// Usage examples
function demonstrateFileSystem(): void {
  console.log("=== File System Demo ===\n");

  const fs = new FileSystem();

  // 1. Directory operations
  console.log("1. Directory Operations:");
  fs.mkdir('/home');
  fs.mkdir('/home/user');
  fs.mkdir('/home/user/documents');
  fs.mkdir('/home/user/pictures');
  
  fs.cd('/home/user');
  console.log('Current directory:', fs.pwd());
  
  fs.mkdir('projects');
  fs.mkdir('projects/web');
  fs.mkdir('projects/mobile');

  // 2. File operations
  console.log("\n2. File Operations:");
  fs.createFile('/home/user/documents/readme.txt', 'This is a readme file');
  fs.createFile('/home/user/documents/notes.md', '# Notes\n\nImportant information');
  fs.createFile('/home/user/projects/web/index.html', '<html><body>Hello World</body></html>');
  
  fs.writeFile('/home/user/documents/readme.txt', 'Updated readme content');
  
  const content = fs.readFile('/home/user/documents/notes.md');
  console.log('File content:', content);

  // 3. Listing and navigation
  console.log("\n3. Listing and Navigation:");
  console.log('Root contents:', fs.ls('/'));
  console.log('Home contents:', fs.ls('/home'));
  console.log('User contents:', fs.ls('/home/user'));
  
  fs.cd('/home/user/projects');
  console.log('Projects contents:', fs.ls('.'));
  
  fs.cd('..');
  console.log('Back to user directory:', fs.pwd());

  // 4. File information
  console.log("\n4. File Information:");
  const stats = fs.stat('/home/user/documents/readme.txt');
  console.log('File stats:', {
    name: stats.name,
    type: stats.type,
    size: FileSystemUtils.formatSize(stats.size),
    permissions: FileSystemUtils.formatPermissions(stats.permissions),
    owner: stats.owner,
    modified: FileSystemUtils.formatDate(stats.modifiedAt)
  });

  // 5. Permissions and ownership
  console.log("\n5. Permissions and Ownership:");
  fs.chmod('/home/user/documents/readme.txt', 0o600);
  fs.chown('/home/user/documents/readme.txt', 'user', 'users');
  
  const updatedStats = fs.stat('/home/user/documents/readme.txt');
  console.log('Updated permissions:', FileSystemUtils.formatPermissions(updatedStats.permissions));
  console.log('Updated ownership:', `${updatedStats.owner}:${updatedStats.group}`);

  // 6. Search operations
  console.log("\n6. Search Operations:");
  const results = fs.find('/home/user', 'readme');
  console.log('Search results for "readme":', results);
  
  const htmlFiles = fs.find('/home/user', '.html');
  console.log('Search results for ".html":', htmlFiles);

  // 7. Disk usage
  console.log("\n7. Disk Usage:");
  const userUsage = fs.du('/home/user');
  console.log('User directory usage:', {
    totalSize: FileSystemUtils.formatSize(userUsage.totalSize),
    files: userUsage.fileCount,
    directories: userUsage.dirCount
  });
  
  const documentsUsage = fs.du('/home/user/documents');
  console.log('Documents usage:', {
    totalSize: FileSystemUtils.formatSize(documentsUsage.totalSize),
    files: documentsUsage.fileCount,
    directories: documentsUsage.dirCount
  });

  // 8. File system statistics
  console.log("\n8. File System Statistics:");
  const fsStats = fs.getStats();
  console.log('File system stats:', {
    totalFiles: fsStats.totalFiles,
    totalDirectories: fsStats.totalDirectories,
    totalSize: FileSystemUtils.formatSize(fsStats.totalSize),
    currentPath: fsStats.currentPath
  });

  // 9. Cleanup
  console.log("\n9. Cleanup:");
  fs.deleteFile('/home/user/documents/notes.md');
  fs.rmdir('/home/user/pictures');
  
  console.log('Final user contents:', fs.ls('/home/user'));
}

// This example demonstrates building a complete file system with TypeScript