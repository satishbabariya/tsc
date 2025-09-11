// CLI Tool Example
// Demonstrates building a command-line tool with TypeScript

import { CLIApplication, ParsedArgs } from './23_systems_programming';

// File operations CLI tool
class FileCLI {
  private cli: CLIApplication;

  constructor() {
    this.cli = new CLIApplication();
    this.setupCommands();
    this.setupOptions();
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
    
    console.log(`Listing files in ${path}${recursive ? ' (recursive)' : ''}`);
    // Simulate file listing
    console.log('file1.txt');
    console.log('file2.txt');
    console.log('subdirectory/');
  }

  private createFile(args: string[]): void {
    const filename = args[0];
    if (!filename) {
      console.error('Error: filename is required');
      return;
    }

    console.log(`Creating file: ${filename}`);
    // Simulate file creation
    console.log(`File ${filename} created successfully`);
  }

  private readFile(args: string[]): void {
    const filename = args[0];
    if (!filename) {
      console.error('Error: filename is required');
      return;
    }

    console.log(`Reading file: ${filename}`);
    // Simulate file reading
    console.log('File contents:');
    console.log('This is the content of the file.');
  }

  private deleteFile(args: string[]): void {
    const filename = args[0];
    if (!filename) {
      console.error('Error: filename is required');
      return;
    }

    const force = this.cli.parseArgs(process.argv).options.get('force') === 'true';
    if (!force) {
      console.log(`Are you sure you want to delete ${filename}? (y/N)`);
      // Simulate user confirmation
      console.log('File deleted');
    } else {
      console.log(`Force deleting: ${filename}`);
      console.log('File deleted');
    }
  }

  private copyFile(args: string[]): void {
    const source = args[0];
    const destination = args[1];
    
    if (!source || !destination) {
      console.error('Error: source and destination are required');
      return;
    }

    console.log(`Copying ${source} to ${destination}`);
    // Simulate file copying
    console.log('File copied successfully');
  }

  private moveFile(args: string[]): void {
    const source = args[0];
    const destination = args[1];
    
    if (!source || !destination) {
      console.error('Error: source and destination are required');
      return;
    }

    console.log(`Moving ${source} to ${destination}`);
    // Simulate file moving
    console.log('File moved successfully');
  }

  private searchFiles(args: string[]): void {
    const pattern = args[0];
    if (!pattern) {
      console.error('Error: search pattern is required');
      return;
    }

    const recursive = this.cli.parseArgs(process.argv).options.get('recursive') === 'true';
    console.log(`Searching for files matching "${pattern}"${recursive ? ' (recursive)' : ''}`);
    
    // Simulate file search
    console.log('file1.txt');
    console.log('file2.txt');
    console.log('subdirectory/file3.txt');
  }

  private showStats(args: string[]): void {
    const path = args[0] || '.';
    console.log(`File statistics for ${path}:`);
    console.log('Total files: 15');
    console.log('Total directories: 3');
    console.log('Total size: 2.5 MB');
    console.log('Last modified: 2024-01-15 10:30:00');
  }

  run(args: string[]): void {
    this.cli.run(args);
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
    console.log('CPU Information:');
    console.log('Model: Intel Core i7-9700K');
    console.log('Cores: 8');
    console.log('Threads: 8');
    console.log('Frequency: 3.60 GHz');
    console.log('Architecture: x86_64');
  }

  private showMemoryInfo(): void {
    console.log('Memory Information:');
    console.log('Total: 16.0 GB');
    console.log('Used: 8.5 GB');
    console.log('Free: 7.5 GB');
    console.log('Available: 7.5 GB');
    console.log('Usage: 53.1%');
  }

  private showDiskInfo(): void {
    console.log('Disk Information:');
    console.log('Device: /dev/sda');
    console.log('Mount: /');
    console.log('Total: 500.0 GB');
    console.log('Used: 300.0 GB');
    console.log('Free: 200.0 GB');
    console.log('Usage: 60.0%');
  }

  private showProcesses(): void {
    console.log('Running Processes:');
    console.log('PID    Name                CPU%    Memory%');
    console.log('1      systemd             0.1     0.5');
    console.log('1234   node                5.2     2.1');
    console.log('5678   chrome              15.3    8.7');
    console.log('9012   vscode              3.4     4.2');
  }

  private showNetworkInfo(): void {
    console.log('Network Information:');
    console.log('Interface: eth0');
    console.log('IP Address: 192.168.1.100');
    console.log('Subnet: 192.168.1.0/24');
    console.log('Gateway: 192.168.1.1');
    console.log('DNS: 8.8.8.8, 8.8.4.4');
  }

  private showUptime(): void {
    console.log('System Uptime:');
    console.log('Uptime: 5 days, 12 hours, 30 minutes');
    console.log('Load Average: 0.45, 0.52, 0.48');
    console.log('Users: 2');
  }

  run(args: string[]): void {
    this.cli.run(args);
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
      console.error('Error: package names are required');
      return;
    }

    const global = this.cli.parseArgs(process.argv).options.get('global') === 'true';
    const dev = this.cli.parseArgs(process.argv).options.get('dev') === 'true';
    
    console.log(`Installing packages: ${packages.join(', ')}`);
    console.log(`Global: ${global}, Dev: ${dev}`);
    
    // Simulate package installation
    for (const pkg of packages) {
      console.log(`Installing ${pkg}...`);
      console.log(`✓ ${pkg} installed successfully`);
    }
  }

  private uninstallPackages(args: string[]): void {
    const packages = args;
    if (packages.length === 0) {
      console.error('Error: package names are required');
      return;
    }

    console.log(`Uninstalling packages: ${packages.join(', ')}`);
    
    // Simulate package uninstallation
    for (const pkg of packages) {
      console.log(`Uninstalling ${pkg}...`);
      console.log(`✓ ${pkg} uninstalled successfully`);
    }
  }

  private listPackages(): void {
    console.log('Installed Packages:');
    console.log('Name                Version    Description');
    console.log('typescript          5.0.0      TypeScript compiler');
    console.log('express             4.18.0     Web framework');
    console.log('lodash              4.17.21    Utility library');
    console.log('@types/node         18.0.0     Node.js type definitions');
  }

  private updatePackages(args: string[]): void {
    const packages = args;
    console.log(`Updating packages: ${packages.length > 0 ? packages.join(', ') : 'all'}`);
    
    // Simulate package updates
    console.log('Checking for updates...');
    console.log('✓ typescript: 5.0.0 → 5.1.0');
    console.log('✓ express: 4.18.0 → 4.19.0');
    console.log('✓ lodash: 4.17.21 → 4.17.22');
  }

  private searchPackages(args: string[]): void {
    const query = args[0];
    if (!query) {
      console.error('Error: search query is required');
      return;
    }

    console.log(`Searching for packages matching "${query}":`);
    console.log('Package Name        Version    Description');
    console.log('typescript          5.0.0      TypeScript compiler');
    console.log('typescript-eslint   6.0.0      ESLint rules for TypeScript');
    console.log('@types/typescript   5.0.0      TypeScript type definitions');
  }

  private showPackageInfo(args: string[]): void {
    const packageName = args[0];
    if (!packageName) {
      console.error('Error: package name is required');
      return;
    }

    console.log(`Package Information: ${packageName}`);
    console.log('Version: 5.0.0');
    console.log('Description: TypeScript compiler');
    console.log('Author: Microsoft');
    console.log('License: Apache-2.0');
    console.log('Repository: https://github.com/Microsoft/TypeScript');
  }

  run(args: string[]): void {
    this.cli.run(args);
  }
}

// Main CLI dispatcher
class MainCLI {
  private cli: CLIApplication;

  constructor() {
    this.cli = new CLIApplication();
    this.setupCommands();
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

  run(args: string[]): void {
    this.cli.run(args);
  }
}

// Main execution
function main(): void {
  const cli = new MainCLI();
  const args = process.argv.slice(2);
  cli.run(args);
}

// Export for use in other modules
export { FileCLI, SystemCLI, PackageCLI, MainCLI };

// Run if this is the main module
if (require.main === module) {
  main();
}