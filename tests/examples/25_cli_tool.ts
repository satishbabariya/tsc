// CLI Tool Example
// Demonstrates building a command-line tool with TypeScript

import {CLIApplication, ParsedArgs} from './23_systems_programming';

// File operations CLI tool
class FileCLI {
    private cli: CLIApplication;

    constructor() {
        this.cli = new CLIApplication();
        this.setupCommands();
        this.setupOptions();
    }

    run(args: string[]): void {
        this.cli.run(args);
    }

    private setupCommands(): void {
        this.cli.addCommand('list', 'List files in directory', (args) => {
            this.listFiles(args);
        });

        this.cli.addCommand('create', 'Create a new file', (args) => {
            this.createFile(args);
        });

        this.cli.addCommand('read', 'Read file contents', (args) => {
            this.readFile(args);
        });

        this.cli.addCommand('delete', 'Delete a file', (args) => {
            this.deleteFile(args);
        });

        this.cli.addCommand('copy', 'Copy a file', (args) => {
            this.copyFile(args);
        });

        this.cli.addCommand('move', 'Move a file', (args) => {
            this.moveFile(args);
        });

        this.cli.addCommand('search', 'Search for files', (args) => {
            this.searchFiles(args);
        });

        this.cli.addCommand('stats', 'Show file statistics', (args) => {
            this.showStats(args);
        });
    }

    private setupOptions(): void {
        this.cli.addOption('r', 'recursive', 'Recursive operation');
        this.cli.addOption('f', 'force', 'Force operation without confirmation');
        this.cli.addOption('v', 'verbose', 'Verbose output');
        this.cli.addOption('h', 'help', 'Show help');
    }

    private listFiles(args: string[]): void {
        const path = args[0] || '.';
        const recursive = this.cli.parseArgs(process.argv).options.get('recursive') === 'true';

        _print(`Listing files in ${path}${recursive ? ' (recursive)' : ''}`);
        // Simulate file listing
        _print('file1.txt');
        _print('file2.txt');
        _print('subdirectory/');
    }

    private createFile(args: string[]): void {
        const filename = args[0];
        if (!filename) {
            _print('Error: filename is required');
            return;
        }

        _print(`Creating file: ${filename}`);
        // Simulate file creation
        _print(`File ${filename} created successfully`);
    }

    private readFile(args: string[]): void {
        const filename = args[0];
        if (!filename) {
            _print('Error: filename is required');
            return;
        }

        _print(`Reading file: ${filename}`);
        // Simulate file reading
        _print('File contents:');
        _print('This is the content of the file.');
    }

    private deleteFile(args: string[]): void {
        const filename = args[0];
        if (!filename) {
            _print('Error: filename is required');
            return;
        }

        const force = this.cli.parseArgs(process.argv).options.get('force') === 'true';
        if (!force) {
            _print(`Are you sure you want to delete ${filename}? (y/N)`);
            // Simulate user confirmation
            _print('File deleted');
        } else {
            _print(`Force deleting: ${filename}`);
            _print('File deleted');
        }
    }

    private copyFile(args: string[]): void {
        const source = args[0];
        const destination = args[1];

        if (!source || !destination) {
            _print('Error: source and destination are required');
            return;
        }

        _print(`Copying ${source} to ${destination}`);
        // Simulate file copying
        _print('File copied successfully');
    }

    private moveFile(args: string[]): void {
        const source = args[0];
        const destination = args[1];

        if (!source || !destination) {
            _print('Error: source and destination are required');
            return;
        }

        _print(`Moving ${source} to ${destination}`);
        // Simulate file moving
        _print('File moved successfully');
    }

    private searchFiles(args: string[]): void {
        const pattern = args[0];
        if (!pattern) {
            _print('Error: search pattern is required');
            return;
        }

        const recursive = this.cli.parseArgs(process.argv).options.get('recursive') === 'true';
        _print(`Searching for files matching "${pattern}"${recursive ? ' (recursive)' : ''}`);

        // Simulate file search
        _print('file1.txt');
        _print('file2.txt');
        _print('subdirectory/file3.txt');
    }

    private showStats(args: string[]): void {
        const path = args[0] || '.';
        _print(`File statistics for ${path}:`);
        _print('Total files: 15');
        _print('Total directories: 3');
        _print('Total size: 2.5 MB');
        _print('Last modified: 2024-01-15 10:30:00');
    }
}

// System monitoring CLI tool
class SystemCLI {
    private cli: CLIApplication;

    constructor() {
        this.cli = new CLIApplication();
        this.setupCommands();
        this.setupOptions();
    }

    run(args: string[]): void {
        this.cli.run(args);
    }

    private setupCommands(): void {
        this.cli.addCommand('cpu', 'Show CPU information', (args) => {
            this.showCPUInfo();
        });

        this.cli.addCommand('memory', 'Show memory information', (args) => {
            this.showMemoryInfo();
        });

        this.cli.addCommand('disk', 'Show disk information', (args) => {
            this.showDiskInfo();
        });

        this.cli.addCommand('processes', 'Show running processes', (args) => {
            this.showProcesses();
        });

        this.cli.addCommand('network', 'Show network information', (args) => {
            this.showNetworkInfo();
        });

        this.cli.addCommand('uptime', 'Show system uptime', (args) => {
            this.showUptime();
        });
    }

    private setupOptions(): void {
        this.cli.addOption('j', 'json', 'Output in JSON format');
        this.cli.addOption('v', 'verbose', 'Verbose output');
        this.cli.addOption('h', 'help', 'Show help');
    }

    private showCPUInfo(): void {
        _print('CPU Information:');
        _print('Model: Intel Core i7-9700K');
        _print('Cores: 8');
        _print('Threads: 8');
        _print('Frequency: 3.60 GHz');
        _print('Architecture: x86_64');
    }

    private showMemoryInfo(): void {
        _print('Memory Information:');
        _print('Total: 16.0 GB');
        _print('Used: 8.5 GB');
        _print('Free: 7.5 GB');
        _print('Available: 7.5 GB');
        _print('Usage: 53.1%');
    }

    private showDiskInfo(): void {
        _print('Disk Information:');
        _print('Device: /dev/sda');
        _print('Mount: /');
        _print('Total: 500.0 GB');
        _print('Used: 300.0 GB');
        _print('Free: 200.0 GB');
        _print('Usage: 60.0%');
    }

    private showProcesses(): void {
        _print('Running Processes:');
        _print('PID    Name                CPU%    Memory%');
        _print('1      systemd             0.1     0.5');
        _print('1234   node                5.2     2.1');
        _print('5678   chrome              15.3    8.7');
        _print('9012   vscode              3.4     4.2');
    }

    private showNetworkInfo(): void {
        _print('Network Information:');
        _print('Interface: eth0');
        _print('IP Address: 192.168.1.100');
        _print('Subnet: 192.168.1.0/24');
        _print('Gateway: 192.168.1.1');
        _print('DNS: 8.8.8.8, 8.8.4.4');
    }

    private showUptime(): void {
        _print('System Uptime:');
        _print('Uptime: 5 days, 12 hours, 30 minutes');
        _print('Load Average: 0.45, 0.52, 0.48');
        _print('Users: 2');
    }
}

// Package manager CLI tool
class PackageCLI {
    private cli: CLIApplication;

    constructor() {
        this.cli = new CLIApplication();
        this.setupCommands();
        this.setupOptions();
    }

    run(args: string[]): void {
        this.cli.run(args);
    }

    private setupCommands(): void {
        this.cli.addCommand('install', 'Install packages', (args) => {
            this.installPackages(args);
        });

        this.cli.addCommand('uninstall', 'Uninstall packages', (args) => {
            this.uninstallPackages(args);
        });

        this.cli.addCommand('list', 'List installed packages', (args) => {
            this.listPackages();
        });

        this.cli.addCommand('update', 'Update packages', (args) => {
            this.updatePackages(args);
        });

        this.cli.addCommand('search', 'Search for packages', (args) => {
            this.searchPackages(args);
        });

        this.cli.addCommand('info', 'Show package information', (args) => {
            this.showPackageInfo(args);
        });
    }

    private setupOptions(): void {
        this.cli.addOption('g', 'global', 'Install globally');
        this.cli.addOption('d', 'dev', 'Install as dev dependency');
        this.cli.addOption('s', 'save', 'Save to package.json');
        this.cli.addOption('h', 'help', 'Show help');
    }

    private installPackages(args: string[]): void {
        const packages = args;
        if (packages.length === 0) {
            _print('Error: package names are required');
            return;
        }

        const global = this.cli.parseArgs(process.argv).options.get('global') === 'true';
        const dev = this.cli.parseArgs(process.argv).options.get('dev') === 'true';

        _print(`Installing packages: ${packages.join(', ')}`);
        _print(`Global: ${global}, Dev: ${dev}`);

        // Simulate package installation
        for (const pkg of packages) {
            _print(`Installing ${pkg}...`);
            _print(`✓ ${pkg} installed successfully`);
        }
    }

    private uninstallPackages(args: string[]): void {
        const packages = args;
        if (packages.length === 0) {
            _print('Error: package names are required');
            return;
        }

        _print(`Uninstalling packages: ${packages.join(', ')}`);

        // Simulate package uninstallation
        for (const pkg of packages) {
            _print(`Uninstalling ${pkg}...`);
            _print(`✓ ${pkg} uninstalled successfully`);
        }
    }

    private listPackages(): void {
        _print('Installed Packages:');
        _print('Name                Version    Description');
        _print('typescript          5.0.0      TypeScript compiler');
        _print('express             4.18.0     Web framework');
        _print('lodash              4.17.21    Utility library');
        _print('@types/node         18.0.0     Node.js type definitions');
    }

    private updatePackages(args: string[]): void {
        const packages = args;
        _print(`Updating packages: ${packages.length > 0 ? packages.join(', ') : 'all'}`);

        // Simulate package updates
        _print('Checking for updates...');
        _print('✓ typescript: 5.0.0 → 5.1.0');
        _print('✓ express: 4.18.0 → 4.19.0');
        _print('✓ lodash: 4.17.21 → 4.17.22');
    }

    private searchPackages(args: string[]): void {
        const query = args[0];
        if (!query) {
            _print('Error: search query is required');
            return;
        }

        _print(`Searching for packages matching "${query}":`);
        _print('Package Name        Version    Description');
        _print('typescript          5.0.0      TypeScript compiler');
        _print('typescript-eslint   6.0.0      ESLint rules for TypeScript');
        _print('@types/typescript   5.0.0      TypeScript type definitions');
    }

    private showPackageInfo(args: string[]): void {
        const packageName = args[0];
        if (!packageName) {
            _print('Error: package name is required');
            return;
        }

        _print(`Package Information: ${packageName}`);
        _print('Version: 5.0.0');
        _print('Description: TypeScript compiler');
        _print('Author: Microsoft');
        _print('License: Apache-2.0');
        _print('Repository: https://github.com/Microsoft/TypeScript');
    }
}

// Main CLI dispatcher
class MainCLI {
    private cli: CLIApplication;

    constructor() {
        this.cli = new CLIApplication();
        this.setupCommands();
    }

    run(args: string[]): void {
        this.cli.run(args);
    }

    private setupCommands(): void {
        this.cli.addCommand('file', 'File operations', (args) => {
            const fileCLI = new FileCLI();
            fileCLI.run(args);
        });

        this.cli.addCommand('system', 'System monitoring', (args) => {
            const systemCLI = new SystemCLI();
            systemCLI.run(args);
        });

        this.cli.addCommand('package', 'Package management', (args) => {
            const packageCLI = new PackageCLI();
            packageCLI.run(args);
        });
    }
}

// Main execution
function main(): void {
    const cli = new MainCLI();
    const args = process.argv.slice(2);
    cli.run(args);
}

// Export for use in other modules
export {FileCLI, SystemCLI, PackageCLI, MainCLI};

// Run if this is the main module
if (require.main === module) {
    main();
}