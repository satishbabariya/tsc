// Compiler Tool Example
// Demonstrates building a compiler and language tools with TypeScript

// Lexer (Tokenizer)
class Lexer {
  private input: string;
  private position: number = 0;
  private currentChar: string | null = null;

  constructor(input: string) {
    this.input = input;
    this.currentChar = input.length > 0 ? input[0] : null;
  }

  private advance(): void {
    this.position++;
    this.currentChar = this.position < this.input.length ? this.input[this.position] : null;
  }

  private skipWhitespace(): void {
    while (this.currentChar && /\s/.test(this.currentChar)) {
      this.advance();
    }
  }

  private readNumber(): Token {
    let value = '';
    while (this.currentChar && /\d/.test(this.currentChar)) {
      value += this.currentChar;
      this.advance();
    }
    return { type: 'NUMBER', value: parseInt(value) };
  }

  private readIdentifier(): Token {
    let value = '';
    while (this.currentChar && /[a-zA-Z_]/.test(this.currentChar)) {
      value += this.currentChar;
      this.advance();
    }
    
    // Check for keywords
    const keywords: Record<string, TokenType> = {
      'let': 'LET',
      'const': 'CONST',
      'function': 'FUNCTION',
      'if': 'IF',
      'else': 'ELSE',
      'while': 'WHILE',
      'for': 'FOR',
      'return': 'RETURN',
      'true': 'TRUE',
      'false': 'FALSE',
      'null': 'NULL'
    };
    
    return {
      type: keywords[value] || 'IDENTIFIER',
      value
    };
  }

  private readString(): Token {
    let value = '';
    this.advance(); // skip opening quote
    
    while (this.currentChar && this.currentChar !== '"') {
      if (this.currentChar === '\\') {
        this.advance();
        switch (this.currentChar) {
          case 'n': value += '\n'; break;
          case 't': value += '\t'; break;
          case 'r': value += '\r'; break;
          case '\\': value += '\\'; break;
          case '"': value += '"'; break;
          default: value += this.currentChar; break;
        }
      } else {
        value += this.currentChar;
      }
      this.advance();
    }
    
    this.advance(); // skip closing quote
    return { type: 'STRING', value };
  }

  getNextToken(): Token {
    while (this.currentChar) {
      if (/\s/.test(this.currentChar)) {
        this.skipWhitespace();
        continue;
      }

      if (/\d/.test(this.currentChar)) {
        return this.readNumber();
      }

      if (/[a-zA-Z_]/.test(this.currentChar)) {
        return this.readIdentifier();
      }

      if (this.currentChar === '"') {
        return this.readString();
      }

      // Handle operators and punctuation
      const char = this.currentChar;
      this.advance();

      switch (char) {
        case '+': return { type: 'PLUS', value: char };
        case '-': return { type: 'MINUS', value: char };
        case '*': return { type: 'MULTIPLY', value: char };
        case '/': return { type: 'DIVIDE', value: char };
        case '=': 
          if (this.currentChar === '=') {
            this.advance();
            return { type: 'EQUALS', value: '==' };
          }
          return { type: 'ASSIGN', value: char };
        case '!':
          if (this.currentChar === '=') {
            this.advance();
            return { type: 'NOT_EQUALS', value: '!=' };
          }
          return { type: 'NOT', value: char };
        case '<':
          if (this.currentChar === '=') {
            this.advance();
            return { type: 'LESS_EQUALS', value: '<=' };
          }
          return { type: 'LESS', value: char };
        case '>':
          if (this.currentChar === '=') {
            this.advance();
            return { type: 'GREATER_EQUALS', value: '>=' };
          }
          return { type: 'GREATER', value: char };
        case '&':
          if (this.currentChar === '&') {
            this.advance();
            return { type: 'AND', value: '&&' };
          }
          return { type: 'BITWISE_AND', value: char };
        case '|':
          if (this.currentChar === '|') {
            this.advance();
            return { type: 'OR', value: '||' };
          }
          return { type: 'BITWISE_OR', value: char };
        case '(': return { type: 'LPAREN', value: char };
        case ')': return { type: 'RPAREN', value: char };
        case '{': return { type: 'LBRACE', value: char };
        case '}': return { type: 'RBRACE', value: char };
        case '[': return { type: 'LBRACKET', value: char };
        case ']': return { type: 'RBRACKET', value: char };
        case ';': return { type: 'SEMICOLON', value: char };
        case ',': return { type: 'COMMA', value: char };
        case '.': return { type: 'DOT', value: char };
        case ':': return { type: 'COLON', value: char };
        default: return { type: 'UNKNOWN', value: char };
      }
    }

    return { type: 'EOF', value: null };
  }
}

type TokenType = 
  | 'NUMBER' | 'STRING' | 'IDENTIFIER'
  | 'LET' | 'CONST' | 'FUNCTION' | 'IF' | 'ELSE' | 'WHILE' | 'FOR' | 'RETURN'
  | 'TRUE' | 'FALSE' | 'NULL'
  | 'PLUS' | 'MINUS' | 'MULTIPLY' | 'DIVIDE'
  | 'ASSIGN' | 'EQUALS' | 'NOT_EQUALS' | 'NOT'
  | 'LESS' | 'LESS_EQUALS' | 'GREATER' | 'GREATER_EQUALS'
  | 'AND' | 'OR' | 'BITWISE_AND' | 'BITWISE_OR'
  | 'LPAREN' | 'RPAREN' | 'LBRACE' | 'RBRACE'
  | 'LBRACKET' | 'RBRACKET' | 'SEMICOLON' | 'COMMA' | 'DOT' | 'COLON'
  | 'EOF' | 'UNKNOWN';

interface Token {
  type: TokenType;
  value: any;
}

// Parser
class Parser {
  private lexer: Lexer;
  private currentToken: Token;

  constructor(input: string) {
    this.lexer = new Lexer(input);
    this.currentToken = this.lexer.getNextToken();
  }

  private advance(): void {
    this.currentToken = this.lexer.getNextToken();
  }

  private expect(tokenType: TokenType): Token {
    if (this.currentToken.type === tokenType) {
      const token = this.currentToken;
      this.advance();
      return token;
    }
    throw new Error(`Expected ${tokenType}, got ${this.currentToken.type}`);
  }

  parse(): ASTNode[] {
    const statements: ASTNode[] = [];
    
    while (this.currentToken.type !== 'EOF') {
      statements.push(this.parseStatement());
    }
    
    return statements;
  }

  private parseStatement(): ASTNode {
    switch (this.currentToken.type) {
      case 'LET':
      case 'CONST':
        return this.parseVariableDeclaration();
      case 'FUNCTION':
        return this.parseFunctionDeclaration();
      case 'IF':
        return this.parseIfStatement();
      case 'WHILE':
        return this.parseWhileStatement();
      case 'FOR':
        return this.parseForStatement();
      case 'RETURN':
        return this.parseReturnStatement();
      case 'LBRACE':
        return this.parseBlockStatement();
      default:
        return this.parseExpressionStatement();
    }
  }

  private parseVariableDeclaration(): ASTNode {
    const kind = this.currentToken.type;
    this.advance(); // skip let/const
    
    const name = this.expect('IDENTIFIER').value;
    this.expect('ASSIGN');
    const value = this.parseExpression();
    this.expect('SEMICOLON');
    
    return {
      type: 'VariableDeclaration',
      kind,
      name,
      value
    };
  }

  private parseFunctionDeclaration(): ASTNode {
    this.advance(); // skip function
    
    const name = this.expect('IDENTIFIER').value;
    this.expect('LPAREN');
    
    const params: string[] = [];
    while (this.currentToken.type !== 'RPAREN') {
      params.push(this.expect('IDENTIFIER').value);
      if (this.currentToken.type === 'COMMA') {
        this.advance();
      }
    }
    this.advance(); // skip )
    
    const body = this.parseBlockStatement();
    
    return {
      type: 'FunctionDeclaration',
      name,
      params,
      body
    };
  }

  private parseIfStatement(): ASTNode {
    this.advance(); // skip if
    this.expect('LPAREN');
    const condition = this.parseExpression();
    this.expect('RPAREN');
    
    const consequent = this.parseStatement();
    let alternate: ASTNode | null = null;
    
    if (this.currentToken.type === 'ELSE') {
      this.advance(); // skip else
      alternate = this.parseStatement();
    }
    
    return {
      type: 'IfStatement',
      condition,
      consequent,
      alternate
    };
  }

  private parseWhileStatement(): ASTNode {
    this.advance(); // skip while
    this.expect('LPAREN');
    const condition = this.parseExpression();
    this.expect('RPAREN');
    const body = this.parseStatement();
    
    return {
      type: 'WhileStatement',
      condition,
      body
    };
  }

  private parseForStatement(): ASTNode {
    this.advance(); // skip for
    this.expect('LPAREN');
    
    const init = this.parseVariableDeclaration();
    const condition = this.parseExpression();
    this.expect('SEMICOLON');
    const update = this.parseExpression();
    this.expect('RPAREN');
    
    const body = this.parseStatement();
    
    return {
      type: 'ForStatement',
      init,
      condition,
      update,
      body
    };
  }

  private parseReturnStatement(): ASTNode {
    this.advance(); // skip return
    const argument = this.parseExpression();
    this.expect('SEMICOLON');
    
    return {
      type: 'ReturnStatement',
      argument
    };
  }

  private parseBlockStatement(): ASTNode {
    this.expect('LBRACE');
    const body: ASTNode[] = [];
    
    while (this.currentToken.type !== 'RBRACE') {
      body.push(this.parseStatement());
    }
    this.advance(); // skip }
    
    return {
      type: 'BlockStatement',
      body
    };
  }

  private parseExpressionStatement(): ASTNode {
    const expression = this.parseExpression();
    this.expect('SEMICOLON');
    
    return {
      type: 'ExpressionStatement',
      expression
    };
  }

  private parseExpression(): ASTNode {
    return this.parseAssignmentExpression();
  }

  private parseAssignmentExpression(): ASTNode {
    const left = this.parseLogicalOrExpression();
    
    if (this.currentToken.type === 'ASSIGN') {
      this.advance();
      const right = this.parseAssignmentExpression();
      return {
        type: 'AssignmentExpression',
        left,
        right
      };
    }
    
    return left;
  }

  private parseLogicalOrExpression(): ASTNode {
    let left = this.parseLogicalAndExpression();
    
    while (this.currentToken.type === 'OR') {
      this.advance();
      const right = this.parseLogicalAndExpression();
      left = {
        type: 'LogicalExpression',
        operator: '||',
        left,
        right
      };
    }
    
    return left;
  }

  private parseLogicalAndExpression(): ASTNode {
    let left = this.parseEqualityExpression();
    
    while (this.currentToken.type === 'AND') {
      this.advance();
      const right = this.parseEqualityExpression();
      left = {
        type: 'LogicalExpression',
        operator: '&&',
        left,
        right
      };
    }
    
    return left;
  }

  private parseEqualityExpression(): ASTNode {
    let left = this.parseRelationalExpression();
    
    while (this.currentToken.type === 'EQUALS' || this.currentToken.type === 'NOT_EQUALS') {
      const operator = this.currentToken.value;
      this.advance();
      const right = this.parseRelationalExpression();
      left = {
        type: 'BinaryExpression',
        operator,
        left,
        right
      };
    }
    
    return left;
  }

  private parseRelationalExpression(): ASTNode {
    let left = this.parseAdditiveExpression();
    
    while (['LESS', 'LESS_EQUALS', 'GREATER', 'GREATER_EQUALS'].includes(this.currentToken.type)) {
      const operator = this.currentToken.value;
      this.advance();
      const right = this.parseAdditiveExpression();
      left = {
        type: 'BinaryExpression',
        operator,
        left,
        right
      };
    }
    
    return left;
  }

  private parseAdditiveExpression(): ASTNode {
    let left = this.parseMultiplicativeExpression();
    
    while (this.currentToken.type === 'PLUS' || this.currentToken.type === 'MINUS') {
      const operator = this.currentToken.value;
      this.advance();
      const right = this.parseMultiplicativeExpression();
      left = {
        type: 'BinaryExpression',
        operator,
        left,
        right
      };
    }
    
    return left;
  }

  private parseMultiplicativeExpression(): ASTNode {
    let left = this.parseUnaryExpression();
    
    while (this.currentToken.type === 'MULTIPLY' || this.currentToken.type === 'DIVIDE') {
      const operator = this.currentToken.value;
      this.advance();
      const right = this.parseUnaryExpression();
      left = {
        type: 'BinaryExpression',
        operator,
        left,
        right
      };
    }
    
    return left;
  }

  private parseUnaryExpression(): ASTNode {
    if (this.currentToken.type === 'NOT' || this.currentToken.type === 'MINUS') {
      const operator = this.currentToken.value;
      this.advance();
      const argument = this.parseUnaryExpression();
      return {
        type: 'UnaryExpression',
        operator,
        argument
      };
    }
    
    return this.parsePrimaryExpression();
  }

  private parsePrimaryExpression(): ASTNode {
    switch (this.currentToken.type) {
      case 'NUMBER':
        const numberToken = this.currentToken;
        this.advance();
        return {
          type: 'Literal',
          value: numberToken.value
        };
      
      case 'STRING':
        const stringToken = this.currentToken;
        this.advance();
        return {
          type: 'Literal',
          value: stringToken.value
        };
      
      case 'TRUE':
      case 'FALSE':
        const boolToken = this.currentToken;
        this.advance();
        return {
          type: 'Literal',
          value: boolToken.type === 'TRUE'
        };
      
      case 'NULL':
        this.advance();
        return {
          type: 'Literal',
          value: null
        };
      
      case 'IDENTIFIER':
        const identifierToken = this.currentToken;
        this.advance();
        return {
          type: 'Identifier',
          name: identifierToken.value
        };
      
      case 'LPAREN':
        this.advance();
        const expression = this.parseExpression();
        this.expect('RPAREN');
        return expression;
      
      default:
        throw new Error(`Unexpected token: ${this.currentToken.type}`);
    }
  }
}

// AST Node types
interface ASTNode {
  type: string;
  [key: string]: any;
}

// Code generator
class CodeGenerator {
  generate(ast: ASTNode[]): string {
    const code: string[] = [];
    
    for (const node of ast) {
      code.push(this.generateNode(node));
    }
    
    return code.join('\n');
  }

  private generateNode(node: ASTNode): string {
    switch (node.type) {
      case 'VariableDeclaration':
        return this.generateVariableDeclaration(node);
      case 'FunctionDeclaration':
        return this.generateFunctionDeclaration(node);
      case 'IfStatement':
        return this.generateIfStatement(node);
      case 'WhileStatement':
        return this.generateWhileStatement(node);
      case 'ForStatement':
        return this.generateForStatement(node);
      case 'ReturnStatement':
        return this.generateReturnStatement(node);
      case 'BlockStatement':
        return this.generateBlockStatement(node);
      case 'ExpressionStatement':
        return this.generateExpressionStatement(node);
      case 'AssignmentExpression':
        return this.generateAssignmentExpression(node);
      case 'BinaryExpression':
        return this.generateBinaryExpression(node);
      case 'UnaryExpression':
        return this.generateUnaryExpression(node);
      case 'LogicalExpression':
        return this.generateLogicalExpression(node);
      case 'Identifier':
        return this.generateIdentifier(node);
      case 'Literal':
        return this.generateLiteral(node);
      default:
        throw new Error(`Unknown node type: ${node.type}`);
    }
  }

  private generateVariableDeclaration(node: ASTNode): string {
    const kind = node.kind.toLowerCase();
    const name = node.name;
    const value = this.generateNode(node.value);
    return `${kind} ${name} = ${value};`;
  }

  private generateFunctionDeclaration(node: ASTNode): string {
    const name = node.name;
    const params = node.params.join(', ');
    const body = this.generateNode(node.body);
    return `function ${name}(${params}) ${body}`;
  }

  private generateIfStatement(node: ASTNode): string {
    const condition = this.generateNode(node.condition);
    const consequent = this.generateNode(node.consequent);
    let code = `if (${condition}) ${consequent}`;
    
    if (node.alternate) {
      code += ` else ${this.generateNode(node.alternate)}`;
    }
    
    return code;
  }

  private generateWhileStatement(node: ASTNode): string {
    const condition = this.generateNode(node.condition);
    const body = this.generateNode(node.body);
    return `while (${condition}) ${body}`;
  }

  private generateForStatement(node: ASTNode): string {
    const init = this.generateNode(node.init);
    const condition = this.generateNode(node.condition);
    const update = this.generateNode(node.update);
    const body = this.generateNode(node.body);
    return `for (${init} ${condition}; ${update}) ${body}`;
  }

  private generateReturnStatement(node: ASTNode): string {
    const argument = node.argument ? this.generateNode(node.argument) : '';
    return `return ${argument};`;
  }

  private generateBlockStatement(node: ASTNode): string {
    const body = node.body.map(stmt => this.generateNode(stmt)).join('\n');
    return `{\n${body.split('\n').map(line => '  ' + line).join('\n')}\n}`;
  }

  private generateExpressionStatement(node: ASTNode): string {
    return this.generateNode(node.expression) + ';';
  }

  private generateAssignmentExpression(node: ASTNode): string {
    const left = this.generateNode(node.left);
    const right = this.generateNode(node.right);
    return `${left} = ${right}`;
  }

  private generateBinaryExpression(node: ASTNode): string {
    const left = this.generateNode(node.left);
    const right = this.generateNode(node.right);
    return `(${left} ${node.operator} ${right})`;
  }

  private generateUnaryExpression(node: ASTNode): string {
    const argument = this.generateNode(node.argument);
    return `${node.operator}${argument}`;
  }

  private generateLogicalExpression(node: ASTNode): string {
    const left = this.generateNode(node.left);
    const right = this.generateNode(node.right);
    return `(${left} ${node.operator} ${right})`;
  }

  private generateIdentifier(node: ASTNode): string {
    return node.name;
  }

  private generateLiteral(node: ASTNode): string {
    if (typeof node.value === 'string') {
      return `"${node.value}"`;
    }
    return String(node.value);
  }
}

// Compiler
class Compiler {
  private lexer: Lexer;
  private parser: Parser;
  private generator: CodeGenerator;

  constructor() {
    this.generator = new CodeGenerator();
  }

  compile(input: string): CompilationResult {
    try {
      this.lexer = new Lexer(input);
      this.parser = new Parser(input);
      
      const ast = this.parser.parse();
      const output = this.generator.generate(ast);
      
      return {
        success: true,
        ast,
        output,
        errors: []
      };
    } catch (error) {
      return {
        success: false,
        ast: [],
        output: '',
        errors: [error.message]
      };
    }
  }

  tokenize(input: string): Token[] {
    const lexer = new Lexer(input);
    const tokens: Token[] = [];
    let token = lexer.getNextToken();
    
    while (token.type !== 'EOF') {
      tokens.push(token);
      token = lexer.getNextToken();
    }
    
    return tokens;
  }
}

interface CompilationResult {
  success: boolean;
  ast: ASTNode[];
  output: string;
  errors: string[];
}

// Language server
class LanguageServer {
  private documents = new Map<string, Document>();
  private diagnostics = new Map<string, Diagnostic[]>();

  openDocument(uri: string, content: string): void {
    this.documents.set(uri, new Document(uri, content));
    this.analyzeDocument(uri);
  }

  updateDocument(uri: string, content: string): void {
    const doc = this.documents.get(uri);
    if (doc) {
      doc.update(content);
      this.analyzeDocument(uri);
    }
  }

  closeDocument(uri: string): void {
    this.documents.delete(uri);
    this.diagnostics.delete(uri);
  }

  getDiagnostics(uri: string): Diagnostic[] {
    return this.diagnostics.get(uri) || [];
  }

  getCompletions(uri: string, position: Position): CompletionItem[] {
    const doc = this.documents.get(uri);
    if (!doc) return [];

    const completions: CompletionItem[] = [];
    
    // Add keyword completions
    const keywords = ['let', 'const', 'function', 'if', 'else', 'while', 'for', 'return'];
    for (const keyword of keywords) {
      completions.push({
        label: keyword,
        kind: 'keyword',
        detail: 'Keyword'
      });
    }

    // Add variable completions
    const variables = this.extractVariables(doc.content);
    for (const variable of variables) {
      completions.push({
        label: variable,
        kind: 'variable',
        detail: 'Variable'
      });
    }

    return completions;
  }

  private analyzeDocument(uri: string): void {
    const doc = this.documents.get(uri);
    if (!doc) return;

    const diagnostics: Diagnostic[] = [];
    const compiler = new Compiler();
    const result = compiler.compile(doc.content);

    if (!result.success) {
      for (const error of result.errors) {
        diagnostics.push({
          range: { start: { line: 0, character: 0 }, end: { line: 0, character: 0 } },
          message: error,
          severity: 'error'
        });
      }
    }

    this.diagnostics.set(uri, diagnostics);
  }

  private extractVariables(content: string): string[] {
    const variables: string[] = [];
    const regex = /(?:let|const)\s+([a-zA-Z_][a-zA-Z0-9_]*)/g;
    let match;
    
    while ((match = regex.exec(content)) !== null) {
      variables.push(match[1]);
    }
    
    return variables;
  }
}

class Document {
  constructor(
    public uri: string,
    public content: string,
    public version: number = 1
  ) {}

  update(content: string): void {
    this.content = content;
    this.version++;
  }
}

interface Diagnostic {
  range: { start: Position; end: Position };
  message: string;
  severity: 'error' | 'warning' | 'info';
}

interface Position {
  line: number;
  character: number;
}

interface CompletionItem {
  label: string;
  kind: string;
  detail: string;
}

// Usage examples
function demonstrateCompilerTool(): void {
  _print("=== Compiler Tool Demo ===\n");

  const compiler = new Compiler();

  // 1. Tokenization
  _print("1. Tokenization:");
  const tokens = compiler.tokenize('let x = 42; function add(a, b) { return a + b; }');
  _print("Tokens:", tokens.map(t => `${t.type}: ${t.value}`).join(', '));

  // 2. Compilation
  _print("\n2. Compilation:");
  const code = `
    let x = 10;
    let y = 20;
    function add(a, b) {
      return a + b;
    }
    let result = add(x, y);
  `;

  const result = compiler.compile(code);
  _print("Compilation success:", result.success);
  _print("Generated code:");
  _print(result.output);

  // 3. Language server
  _print("\n3. Language Server:");
  const languageServer = new LanguageServer();
  languageServer.openDocument('file:///test.ts', code);
  
  const diagnostics = languageServer.getDiagnostics('file:///test.ts');
  _print("Diagnostics:", diagnostics);
  
  const completions = languageServer.getCompletions('file:///test.ts', { line: 0, character: 0 });
  _print("Completions:", completions.slice(0, 5)); // Show first 5
}

// This example demonstrates building a complete compiler toolchain with TypeScript