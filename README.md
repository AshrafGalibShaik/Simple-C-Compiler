# Simple C Compiler

A basic compiler written in C that demonstrates the fundamental phases of compilation: lexical analysis, parsing, semantic analysis, and code generation. This educational project compiles a simple custom language into executable C code.

## Features

- **Lexical Analysis**: Tokenizes source code into meaningful symbols
- **Recursive Descent Parser**: Builds Abstract Syntax Trees (AST) with proper operator precedence
- **Symbol Table**: Manages variables and their values
- **Code Generation**: Outputs equivalent C code
- **Interpreter Mode**: Executes programs directly for testing
- **Error Handling**: Provides line numbers and descriptive error messages

## Supported Language

The compiler supports a simple imperative language with the following features:

### Data Types
- Integers only

### Operators
- Arithmetic: `+`, `-`, `*`, `/`
- Assignment: `=`
- Parentheses: `(`, `)`

### Statements
- **Variable Assignment**: `variable = expression`
- **Print Statement**: `print expression`
- **Comments**: Not supported (yet)

### Example Program
```
x = 10
y = 20
z = (x + y) * 2
print z
print x + y
```

## Installation

### Prerequisites
- GCC compiler
- Standard C library

### Build Instructions
1. Clone or download the source code
2. Compile the compiler:
   ```bash
   gcc -o compiler compiler.c
   ```

## Usage

### Basic Usage
```bash
./compiler <source_file>
```

### Example Workflow
1. **Create a source file** (`example.src`):
   ```
   a = 5
   b = 10
   result = a * b + 15
   print result
   ```

2. **Compile the source**:
   ```bash
   ./compiler example.src
   ```

3. **The compiler will**:
   - Execute the program immediately (interpreter mode)
   - Generate `example.src.c` containing equivalent C code

4. **Compile and run the generated C code**:
   ```bash
   gcc -o program example.src.c
   ./program
   ```

### Command Line Options
Currently supports only one argument - the source file path.

## Architecture

### Compilation Phases

1. **Lexical Analysis (Tokenizer)**
   - Converts source code into tokens
   - Handles keywords, identifiers, numbers, operators
   - Tracks line numbers for error reporting

2. **Syntax Analysis (Parser)**
   - Recursive descent parser
   - Builds Abstract Syntax Tree (AST)
   - Implements operator precedence:
     - `*`, `/` (higher precedence)
     - `+`, `-` (lower precedence)
     - `()` (highest precedence)

3. **Semantic Analysis**
   - Variable declaration and usage tracking
   - Basic type checking
   - Symbol table management

4. **Code Generation**
   - Traverses AST to generate C code
   - Handles variable declarations
   - Converts expressions to C syntax

### File Structure
```
compiler.c          # Main compiler source
├── Lexer           # Tokenization functions
├── Parser          # AST construction
├── Interpreter     # Direct execution
├── Code Generator  # C code output
└── Symbol Table    # Variable management
```

## Grammar

The language follows this context-free grammar:

```
program     → statement*
statement   → assignment | print_stmt
assignment  → IDENTIFIER '=' expression
print_stmt  → 'print' expression
expression  → term (('+' | '-') term)*
term        → factor (('*' | '/') factor)*
factor      → NUMBER | IDENTIFIER | '(' expression ')'
```

## Examples

### Example 1: Basic Arithmetic
**Input** (`math.src`):
```
x = 10
y = 5
sum = x + y
difference = x - y
product = x * y
quotient = x / y
print sum
print difference
print product
print quotient
```

**Output**:
```
15
5
50
2
```

### Example 2: Complex Expressions
**Input** (`complex.src`):
```
a = 2
b = 3
c = 4
result = a + b * c - (a + b)
print result
```

**Output**:
```
9
```

### Example 3: Variable Reuse
**Input** (`reuse.src`):
```
counter = 1
counter = counter + 1
counter = counter * 2
print counter
```

**Output**:
```
4
```

## Error Handling

The compiler provides helpful error messages with line numbers:

- **Syntax Errors**: Invalid token sequences
- **Undefined Variables**: Using variables before declaration
- **Division by Zero**: Runtime error detection
- **Unexpected Tokens**: Malformed expressions

### Example Error Messages
```
Error: Undefined variable 'x' at line 2
Error: Expected ')' at line 3
Error: Division by zero
Error: Unknown character '$' at line 1
```

## Limitations

- **Data Types**: Only supports integers
- **Control Flow**: No if/else, loops, or functions
- **Comments**: No comment support
- **Strings**: No string literals or operations
- **Arrays**: No array support
- **Scope**: Global scope only

## Future Enhancements

Potential improvements for learning purposes:

1. **Language Features**:
   - Boolean data type and operations
   - String literals and operations
   - Control flow (if/else, while, for)
   - Functions and procedures
   - Arrays and indexing

2. **Compiler Features**:
   - Optimization passes
   - Better error recovery
   - Multiple file support
   - Assembly code generation
   - Debugging information

3. **Development Tools**:
   - Unit tests
   - Benchmarking
   - Documentation generation

## Contributing

This is an educational project. Feel free to:
- Add new language features
- Improve error messages
- Optimize code generation
- Add more examples
- Fix bugs

## Learning Resources

To understand compiler theory better:
- "Compilers: Principles, Techniques, and Tools" by Aho, Lam, Sethi, and Ullman
- "Modern Compiler Implementation in C" by Andrew Appel
- Online courses on compiler construction

## License

This project is provided for educational purposes. Feel free to use, modify, and distribute.

---

**Note**: This is a educational compiler designed for learning purposes. For production use, consider established compiler frameworks like LLVM or existing programming languages.
