// Network Protocol Example
// Demonstrates building network protocols and communication systems with TypeScript

// Protocol definitions
enum MessageType {
  HANDSHAKE = 0x01,
  DATA = 0x02,
  ACK = 0x03,
  NACK = 0x04,
  HEARTBEAT = 0x05,
  DISCONNECT = 0x06
}

interface ProtocolMessage {
  type: MessageType;
  sequence: number;
  timestamp: number;
  payload: Uint8Array;
  checksum: number;
}

// Message serializer/deserializer
class MessageSerializer {
  static serialize(message: ProtocolMessage): Uint8Array {
    const buffer = new ArrayBuffer(16 + message.payload.length);
    const view = new DataView(buffer);
    
    let offset = 0;
    
    // Header
    view.setUint8(offset++, message.type);
    view.setUint32(offset, message.sequence);
    offset += 4;
    view.setUint32(offset, message.timestamp);
    offset += 4;
    view.setUint32(offset, message.payload.length);
    offset += 4;
    
    // Payload
    const payloadView = new Uint8Array(buffer, offset, message.payload.length);
    payloadView.set(message.payload);
    offset += message.payload.length;
    
    // Checksum
    const checksum = this.calculateChecksum(new Uint8Array(buffer, 0, offset));
    view.setUint32(offset, checksum);
    
    return new Uint8Array(buffer);
  }

  static deserialize(data: Uint8Array): ProtocolMessage {
    if (data.length < 16) {
      throw new Error('Invalid message: too short');
    }
    
    const view = new DataView(data.buffer);
    let offset = 0;
    
    const type = view.getUint8(offset++);
    const sequence = view.getUint32(offset);
    offset += 4;
    const timestamp = view.getUint32(offset);
    offset += 4;
    const payloadLength = view.getUint32(offset);
    offset += 4;
    
    if (data.length < 16 + payloadLength) {
      throw new Error('Invalid message: payload truncated');
    }
    
    const payload = new Uint8Array(data.buffer, offset, payloadLength);
    offset += payloadLength;
    
    const checksum = view.getUint32(offset);
    
    // Verify checksum
    const calculatedChecksum = this.calculateChecksum(new Uint8Array(data.buffer, 0, offset));
    if (checksum !== calculatedChecksum) {
      throw new Error('Invalid message: checksum mismatch');
    }
    
    return {
      type,
      sequence,
      timestamp,
      payload,
      checksum
    };
  }

  private static calculateChecksum(data: Uint8Array): number {
    let checksum = 0;
    for (let i = 0; i < data.length; i++) {
      checksum = (checksum + data[i]) & 0xFFFFFFFF;
    }
    return checksum;
  }
}

// Connection manager
class ConnectionManager {
  private connections = new Map<string, Connection>();
  private nextConnectionId = 1;

  createConnection(address: string, port: number): Connection {
    const id = `conn_${this.nextConnectionId++}`;
    const connection = new Connection(id, address, port);
    this.connections.set(id, connection);
    return connection;
  }

  getConnection(id: string): Connection | undefined {
    return this.connections.get(id);
  }

  removeConnection(id: string): void {
    const connection = this.connections.get(id);
    if (connection) {
      connection.close();
      this.connections.delete(id);
    }
  }

  getAllConnections(): Connection[] {
    return Array.from(this.connections.values());
  }

  getConnectionCount(): number {
    return this.connections.size;
  }
}

class Connection {
  private socket: TCPSocket;
  private messageQueue: ProtocolMessage[] = [];
  private sequenceNumber = 0;
  private lastHeartbeat = 0;
  private heartbeatInterval = 30000; // 30 seconds
  private isConnected = false;

  constructor(
    public id: string,
    public address: string,
    public port: number
  ) {
    this.socket = new TCPSocket(address, port);
  }

  async connect(): Promise<void> {
    try {
      await this.socket.connect();
      this.isConnected = true;
      this.sendHandshake();
      this.startHeartbeat();
      console.log(`Connected to ${this.address}:${this.port}`);
    } catch (error) {
      console.error(`Failed to connect to ${this.address}:${this.port}:`, error);
      throw error;
    }
  }

  async disconnect(): Promise<void> {
    this.isConnected = false;
    await this.sendMessage({
      type: MessageType.DISCONNECT,
      sequence: this.getNextSequence(),
      timestamp: Date.now(),
      payload: new Uint8Array(0),
      checksum: 0
    });
    await this.socket.close();
    console.log(`Disconnected from ${this.address}:${this.port}`);
  }

  async sendData(data: Uint8Array): Promise<void> {
    const message: ProtocolMessage = {
      type: MessageType.DATA,
      sequence: this.getNextSequence(),
      timestamp: Date.now(),
      payload: data,
      checksum: 0
    };
    
    await this.sendMessage(message);
  }

  async sendMessage(message: ProtocolMessage): Promise<void> {
    const serialized = MessageSerializer.serialize(message);
    await this.socket.send(serialized);
  }

  async receiveMessage(): Promise<ProtocolMessage | null> {
    try {
      const data = await this.socket.receive();
      if (data.length === 0) {
        return null;
      }
      
      const message = MessageSerializer.deserialize(data);
      await this.handleMessage(message);
      return message;
    } catch (error) {
      console.error('Error receiving message:', error);
      return null;
    }
  }

  private async handleMessage(message: ProtocolMessage): Promise<void> {
    switch (message.type) {
      case MessageType.HANDSHAKE:
        await this.handleHandshake(message);
        break;
      case MessageType.DATA:
        await this.handleData(message);
        break;
      case MessageType.ACK:
        await this.handleAck(message);
        break;
      case MessageType.NACK:
        await this.handleNack(message);
        break;
      case MessageType.HEARTBEAT:
        await this.handleHeartbeat(message);
        break;
      case MessageType.DISCONNECT:
        await this.handleDisconnect(message);
        break;
    }
  }

  private async handleHandshake(message: ProtocolMessage): Promise<void> {
    console.log('Received handshake');
    await this.sendMessage({
      type: MessageType.ACK,
      sequence: this.getNextSequence(),
      timestamp: Date.now(),
      payload: new Uint8Array(0),
      checksum: 0
    });
  }

  private async handleData(message: ProtocolMessage): Promise<void> {
    console.log(`Received data: ${message.payload.length} bytes`);
    this.messageQueue.push(message);
    
    // Send ACK
    await this.sendMessage({
      type: MessageType.ACK,
      sequence: this.getNextSequence(),
      timestamp: Date.now(),
      payload: new Uint8Array(0),
      checksum: 0
    });
  }

  private async handleAck(message: ProtocolMessage): Promise<void> {
    console.log('Received ACK');
  }

  private async handleNack(message: ProtocolMessage): Promise<void> {
    console.log('Received NACK');
  }

  private async handleHeartbeat(message: ProtocolMessage): Promise<void> {
    this.lastHeartbeat = Date.now();
    console.log('Received heartbeat');
  }

  private async handleDisconnect(message: ProtocolMessage): Promise<void> {
    console.log('Received disconnect');
    this.isConnected = false;
  }

  private sendHandshake(): void {
    const handshake: ProtocolMessage = {
      type: MessageType.HANDSHAKE,
      sequence: this.getNextSequence(),
      timestamp: Date.now(),
      payload: new Uint8Array(0),
      checksum: 0
    };
    
    this.sendMessage(handshake);
  }

  private startHeartbeat(): void {
    setInterval(() => {
      if (this.isConnected) {
        const heartbeat: ProtocolMessage = {
          type: MessageType.HEARTBEAT,
          sequence: this.getNextSequence(),
          timestamp: Date.now(),
          payload: new Uint8Array(0),
          checksum: 0
        };
        
        this.sendMessage(heartbeat);
      }
    }, this.heartbeatInterval);
  }

  private getNextSequence(): number {
    return ++this.sequenceNumber;
  }

  async close(): Promise<void> {
    this.isConnected = false;
    await this.socket.close();
  }

  getStatus(): ConnectionStatus {
    return {
      id: this.id,
      address: this.address,
      port: this.port,
      isConnected: this.isConnected,
      sequenceNumber: this.sequenceNumber,
      lastHeartbeat: this.lastHeartbeat,
      queuedMessages: this.messageQueue.length
    };
  }
}

interface ConnectionStatus {
  id: string;
  address: string;
  port: number;
  isConnected: boolean;
  sequenceNumber: number;
  lastHeartbeat: number;
  queuedMessages: number;
}

// TCP Socket implementation
class TCPSocket {
  private socket: any; // Would be actual socket implementation
  private connected = false;

  constructor(private address: string, private port: number) {}

  async connect(): Promise<void> {
    console.log(`Connecting to ${this.address}:${this.port}`);
    // Simulate connection
    this.connected = true;
  }

  async send(data: Uint8Array): Promise<void> {
    if (!this.connected) {
      throw new Error('Socket not connected');
    }
    console.log(`Sending ${data.length} bytes`);
  }

  async receive(): Promise<Uint8Array> {
    if (!this.connected) {
      throw new Error('Socket not connected');
    }
    // Simulate receiving data
    return new Uint8Array(0);
  }

  async close(): Promise<void> {
    this.connected = false;
    console.log('Socket closed');
  }
}

// Protocol server
class ProtocolServer {
  private connectionManager: ConnectionManager;
  private isRunning = false;
  private port: number;

  constructor(port: number) {
    this.port = port;
    this.connectionManager = new ConnectionManager();
  }

  async start(): Promise<void> {
    console.log(`Starting protocol server on port ${this.port}`);
    this.isRunning = true;
    
    // Simulate server loop
    while (this.isRunning) {
      await this.handleConnections();
      await this.sleep(100); // 100ms loop
    }
  }

  async stop(): Promise<void> {
    console.log('Stopping protocol server');
    this.isRunning = false;
    
    // Close all connections
    const connections = this.connectionManager.getAllConnections();
    for (const connection of connections) {
      await connection.close();
    }
  }

  private async handleConnections(): Promise<void> {
    const connections = this.connectionManager.getAllConnections();
    
    for (const connection of connections) {
      try {
        const message = await connection.receiveMessage();
        if (message) {
          await this.processMessage(connection, message);
        }
      } catch (error) {
        console.error(`Error handling connection ${connection.id}:`, error);
        this.connectionManager.removeConnection(connection.id);
      }
    }
  }

  private async processMessage(connection: Connection, message: ProtocolMessage): Promise<void> {
    switch (message.type) {
      case MessageType.DATA:
        await this.handleDataMessage(connection, message);
        break;
      case MessageType.HEARTBEAT:
        await this.handleHeartbeatMessage(connection, message);
        break;
      default:
        console.log(`Received message type: ${message.type}`);
    }
  }

  private async handleDataMessage(connection: Connection, message: ProtocolMessage): Promise<void> {
    console.log(`Processing data message from ${connection.id}: ${message.payload.length} bytes`);
    
    // Echo the data back
    await connection.sendData(message.payload);
  }

  private async handleHeartbeatMessage(connection: Connection, message: ProtocolMessage): Promise<void> {
    console.log(`Received heartbeat from ${connection.id}`);
  }

  private async sleep(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  getConnectionManager(): ConnectionManager {
    return this.connectionManager;
  }
}

// Protocol client
class ProtocolClient {
  private connectionManager: ConnectionManager;
  private connections = new Map<string, Connection>();

  constructor() {
    this.connectionManager = new ConnectionManager();
  }

  async connectToServer(address: string, port: number): Promise<Connection> {
    const connection = this.connectionManager.createConnection(address, port);
    await connection.connect();
    this.connections.set(connection.id, connection);
    return connection;
  }

  async sendData(connectionId: string, data: Uint8Array): Promise<void> {
    const connection = this.connections.get(connectionId);
    if (!connection) {
      throw new Error(`Connection ${connectionId} not found`);
    }
    
    await connection.sendData(data);
  }

  async disconnectFromServer(connectionId: string): Promise<void> {
    const connection = this.connections.get(connectionId);
    if (connection) {
      await connection.disconnect();
      this.connections.delete(connectionId);
      this.connectionManager.removeConnection(connectionId);
    }
  }

  getConnections(): Connection[] {
    return Array.from(this.connections.values());
  }
}

// Load balancer
class LoadBalancer {
  private servers: ServerInfo[] = [];
  private currentIndex = 0;

  addServer(address: string, port: number, weight: number = 1): void {
    this.servers.push({ address, port, weight, active: true });
  }

  removeServer(address: string, port: number): void {
    this.servers = this.servers.filter(s => !(s.address === address && s.port === port));
  }

  getNextServer(): ServerInfo | null {
    const activeServers = this.servers.filter(s => s.active);
    if (activeServers.length === 0) {
      return null;
    }

    // Round-robin selection
    const server = activeServers[this.currentIndex % activeServers.length];
    this.currentIndex++;
    return server;
  }

  getServerStats(): ServerStats[] {
    return this.servers.map(server => ({
      address: server.address,
      port: server.port,
      weight: server.weight,
      active: server.active,
      connections: 0, // Would track actual connections
      load: 0 // Would track actual load
    }));
  }
}

interface ServerInfo {
  address: string;
  port: number;
  weight: number;
  active: boolean;
}

interface ServerStats {
  address: string;
  port: number;
  weight: number;
  active: boolean;
  connections: number;
  load: number;
}

// Usage examples
async function demonstrateNetworkProtocol(): Promise<void> {
  console.log("=== Network Protocol Demo ===\n");

  // 1. Message serialization
  console.log("1. Message Serialization:");
  const message: ProtocolMessage = {
    type: MessageType.DATA,
    sequence: 1,
    timestamp: Date.now(),
    payload: new TextEncoder().encode("Hello, World!"),
    checksum: 0
  };

  const serialized = MessageSerializer.serialize(message);
  console.log(`Serialized message: ${serialized.length} bytes`);

  const deserialized = MessageSerializer.deserialize(serialized);
  console.log(`Deserialized message type: ${deserialized.type}`);
  console.log(`Deserialized payload: ${new TextDecoder().decode(deserialized.payload)}`);

  // 2. Connection management
  console.log("\n2. Connection Management:");
  const connectionManager = new ConnectionManager();
  
  const conn1 = connectionManager.createConnection("127.0.0.1", 8080);
  const conn2 = connectionManager.createConnection("127.0.0.1", 8081);
  
  console.log(`Created ${connectionManager.getConnectionCount()} connections`);

  // 3. Protocol server
  console.log("\n3. Protocol Server:");
  const server = new ProtocolServer(8080);
  
  // Start server in background
  server.start().catch(console.error);
  
  // Wait a bit for server to start
  await new Promise(resolve => setTimeout(resolve, 100));

  // 4. Protocol client
  console.log("\n4. Protocol Client:");
  const client = new ProtocolClient();
  
  try {
    const connection = await client.connectToServer("127.0.0.1", 8080);
    console.log(`Connected to server: ${connection.id}`);
    
    await client.sendData(connection.id, new TextEncoder().encode("Test message"));
    console.log("Sent test message");
    
    await client.disconnectFromServer(connection.id);
    console.log("Disconnected from server");
  } catch (error) {
    console.error("Client error:", error);
  }

  // 5. Load balancer
  console.log("\n5. Load Balancer:");
  const loadBalancer = new LoadBalancer();
  
  loadBalancer.addServer("127.0.0.1", 8080, 1);
  loadBalancer.addServer("127.0.0.1", 8081, 2);
  loadBalancer.addServer("127.0.0.1", 8082, 1);
  
  const server1 = loadBalancer.getNextServer();
  const server2 = loadBalancer.getNextServer();
  const server3 = loadBalancer.getNextServer();
  
  console.log(`Selected servers: ${server1?.port}, ${server2?.port}, ${server3?.port}`);
  
  const stats = loadBalancer.getServerStats();
  console.log("Server stats:", stats);

  // Stop server
  await server.stop();
}

// This example demonstrates building network protocols and communication systems with TypeScript