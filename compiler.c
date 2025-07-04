#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_PRINT,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_NEWLINE
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[256];
    int line;
} Token;

// AST Node types
typedef enum {
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_BINARY_OP,
    NODE_ASSIGN,
    NODE_PRINT
} NodeType;

// AST Node structure
typedef struct ASTNode {
    NodeType type;
    char value[256];
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

// Symbol table for variables
typedef struct {
    char name[256];
    int value;
} Variable;

Variable variables[100];
int var_count = 0;

// Global variables for parsing
char* source_code;
int current_pos = 0;
int current_line = 1;
Token current_token;

// Function declarations
void next_token();
ASTNode* parse_expression();
ASTNode* parse_term();
ASTNode* parse_factor();
ASTNode* parse_statement();
void compile_node(ASTNode* node, FILE* output);
int get_variable_value(const char* name);
void set_variable_value(const char* name, int value);

// Lexer - tokenizes the input
void next_token() {
    // Skip whitespace
    while (source_code[current_pos] == ' ' || source_code[current_pos] == '\t') {
        current_pos++;
    }
    
    if (source_code[current_pos] == '\0') {
        current_token.type = TOKEN_EOF;
        return;
    }
    
    if (source_code[current_pos] == '\n') {
        current_token.type = TOKEN_NEWLINE;
        current_line++;
        current_pos++;
        return;
    }
    
    // Numbers
    if (isdigit(source_code[current_pos])) {
        current_token.type = TOKEN_NUMBER;
        int i = 0;
        while (isdigit(source_code[current_pos])) {
            current_token.value[i++] = source_code[current_pos++];
        }
        current_token.value[i] = '\0';
        return;
    }
    
    // Identifiers and keywords
    if (isalpha(source_code[current_pos])) {
        current_token.type = TOKEN_IDENTIFIER;
        int i = 0;
        while (isalnum(source_code[current_pos]) || source_code[current_pos] == '_') {
            current_token.value[i++] = source_code[current_pos++];
        }
        current_token.value[i] = '\0';
        
        // Check for keywords
        if (strcmp(current_token.value, "print") == 0) {
            current_token.type = TOKEN_PRINT;
        }
        return;
    }
    
    // Single character tokens
    switch (source_code[current_pos]) {
        case '=':
            current_token.type = TOKEN_ASSIGN;
            current_token.value[0] = '=';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        case '+':
            current_token.type = TOKEN_PLUS;
            current_token.value[0] = '+';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        case '-':
            current_token.type = TOKEN_MINUS;
            current_token.value[0] = '-';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        case '*':
            current_token.type = TOKEN_MULTIPLY;
            current_token.value[0] = '*';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        case '/':
            current_token.type = TOKEN_DIVIDE;
            current_token.value[0] = '/';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        case '(':
            current_token.type = TOKEN_LPAREN;
            current_token.value[0] = '(';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        case ')':
            current_token.type = TOKEN_RPAREN;
            current_token.value[0] = ')';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        case ';':
            current_token.type = TOKEN_SEMICOLON;
            current_token.value[0] = ';';
            current_token.value[1] = '\0';
            current_pos++;
            break;
        default:
            printf("Error: Unknown character '%c' at line %d\n", source_code[current_pos], current_line);
            exit(1);
    }
    
    current_token.line = current_line;
}

// Create AST nodes
ASTNode* create_node(NodeType type, const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    strcpy(node->value, value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Parser - builds Abstract Syntax Tree
ASTNode* parse_factor() {
    if (current_token.type == TOKEN_NUMBER) {
        ASTNode* node = create_node(NODE_NUMBER, current_token.value);
        next_token();
        return node;
    }
    
    if (current_token.type == TOKEN_IDENTIFIER) {
        ASTNode* node = create_node(NODE_IDENTIFIER, current_token.value);
        next_token();
        return node;
    }
    
    if (current_token.type == TOKEN_LPAREN) {
        next_token(); // consume '('
        ASTNode* node = parse_expression();
        if (current_token.type != TOKEN_RPAREN) {
            printf("Error: Expected ')' at line %d\n", current_line);
            exit(1);
        }
        next_token(); // consume ')'
        return node;
    }
    
    printf("Error: Unexpected token at line %d\n", current_line);
    exit(1);
}

ASTNode* parse_term() {
    ASTNode* left = parse_factor();
    
    while (current_token.type == TOKEN_MULTIPLY || current_token.type == TOKEN_DIVIDE) {
        char op = current_token.value[0];
        next_token();
        ASTNode* right = parse_factor();
        
        ASTNode* node = create_node(NODE_BINARY_OP, "");
        node->value[0] = op;
        node->value[1] = '\0';
        node->left = left;
        node->right = right;
        left = node;
    }
    
    return left;
}

ASTNode* parse_expression() {
    ASTNode* left = parse_term();
    
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        char op = current_token.value[0];
        next_token();
        ASTNode* right = parse_term();
        
        ASTNode* node = create_node(NODE_BINARY_OP, "");
        node->value[0] = op;
        node->value[1] = '\0';
        node->left = left;
        node->right = right;
        left = node;
    }
    
    return left;
}

ASTNode* parse_statement() {
    if (current_token.type == TOKEN_IDENTIFIER) {
        char var_name[256];
        strcpy(var_name, current_token.value);
        next_token();
        
        if (current_token.type == TOKEN_ASSIGN) {
            next_token(); // consume '='
            ASTNode* expr = parse_expression();
            
            ASTNode* assign_node = create_node(NODE_ASSIGN, var_name);
            assign_node->right = expr;
            return assign_node;
        }
    }
    
    if (current_token.type == TOKEN_PRINT) {
        next_token(); // consume 'print'
        ASTNode* expr = parse_expression();
        
        ASTNode* print_node = create_node(NODE_PRINT, "");
        print_node->right = expr;
        return print_node;
    }
    
    printf("Error: Invalid statement at line %d\n", current_line);
    exit(1);
}

// Variable management
int get_variable_value(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    printf("Error: Undefined variable '%s'\n", name);
    exit(1);
}

void set_variable_value(const char* name, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    }
    // Add new variable
    strcpy(variables[var_count].name, name);
    variables[var_count].value = value;
    var_count++;
}

// Interpreter - evaluates the AST
int evaluate_expression(ASTNode* node) {
    if (node->type == NODE_NUMBER) {
        return atoi(node->value);
    }
    
    if (node->type == NODE_IDENTIFIER) {
        return get_variable_value(node->value);
    }
    
    if (node->type == NODE_BINARY_OP) {
        int left_val = evaluate_expression(node->left);
        int right_val = evaluate_expression(node->right);
        
        switch (node->value[0]) {
            case '+': return left_val + right_val;
            case '-': return left_val - right_val;
            case '*': return left_val * right_val;
            case '/': 
                if (right_val == 0) {
                    printf("Error: Division by zero\n");
                    exit(1);
                }
                return left_val / right_val;
        }
    }
    
    printf("Error: Invalid expression node\n");
    exit(1);
}

void execute_statement(ASTNode* node) {
    if (node->type == NODE_ASSIGN) {
        int value = evaluate_expression(node->right);
        set_variable_value(node->value, value);
    } else if (node->type == NODE_PRINT) {
        int value = evaluate_expression(node->right);
        printf("%d\n", value);
    }
}

// Code generator - generates C code
void generate_expression(ASTNode* node, FILE* output) {
    if (node->type == NODE_NUMBER) {
        fprintf(output, "%s", node->value);
    } else if (node->type == NODE_IDENTIFIER) {
        fprintf(output, "%s", node->value);
    } else if (node->type == NODE_BINARY_OP) {
        fprintf(output, "(");
        generate_expression(node->left, output);
        fprintf(output, " %c ", node->value[0]);
        generate_expression(node->right, output);
        fprintf(output, ")");
    }
}

void generate_statement(ASTNode* node, FILE* output) {
    if (node->type == NODE_ASSIGN) {
        fprintf(output, "    %s = ", node->value);
        generate_expression(node->right, output);
        fprintf(output, ";\n");
    } else if (node->type == NODE_PRINT) {
        fprintf(output, "    printf(\"%%d\\n\", ");
        generate_expression(node->right, output);
        fprintf(output, ");\n");
    }
}

// Main compiler function
void compile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    // Read entire file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    source_code = (char*)malloc(file_size + 1);
    fread(source_code, 1, file_size, file);
    source_code[file_size] = '\0';
    fclose(file);
    
    // Initialize lexer
    current_pos = 0;
    current_line = 1;
    next_token();
    
    // Create output file
    char output_filename[256];
    strcpy(output_filename, filename);
    strcat(output_filename, ".c");
    
    FILE* output = fopen(output_filename, "w");
    if (!output) {
        printf("Error: Cannot create output file\n");
        return;
    }
    
    // Generate C code header
    fprintf(output, "#include <stdio.h>\n\n");
    fprintf(output, "int main() {\n");
    
    // Declare variables
    fprintf(output, "    int ");
    int first_var = 1;
    
    // Parse and compile each statement
    while (current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_NEWLINE) {
            next_token();
            continue;
        }
        
        ASTNode* stmt = parse_statement();
        
        // Add variable declaration if needed
        if (stmt->type == NODE_ASSIGN) {
            int found = 0;
            for (int i = 0; i < var_count; i++) {
                if (strcmp(variables[i].name, stmt->value) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                if (!first_var) fprintf(output, ", ");
                fprintf(output, "%s", stmt->value);
                first_var = 0;
                strcpy(variables[var_count].name, stmt->value);
                var_count++;
            }
        }
        
        // Skip semicolon if present
        if (current_token.type == TOKEN_SEMICOLON) {
            next_token();
        }
        
        // Execute for interpreter mode
        execute_statement(stmt);
        
        free(stmt);
    }
    
    fprintf(output, ";\n\n");
    
    // Reset and recompile for code generation
    current_pos = 0;
    current_line = 1;
    next_token();
    
    while (current_token.type != TOKEN_EOF) {
        if (current_token.type == TOKEN_NEWLINE) {
            next_token();
            continue;
        }
        
        ASTNode* stmt = parse_statement();
        generate_statement(stmt, output);
        
        if (current_token.type == TOKEN_SEMICOLON) {
            next_token();
        }
        
        free(stmt);
    }
    
    fprintf(output, "    return 0;\n");
    fprintf(output, "}\n");
    
    fclose(output);
    free(source_code);
    
    printf("Compilation complete. Output written to '%s'\n", output_filename);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <source_file>\n", argv[0]);
        printf("\nExample source code:\n");
        printf("x = 10\n");
        printf("y = 20\n");
        printf("z = x + y * 2\n");
        printf("print z\n");
        return 1;
    }
    
    compile(argv[1]);
    return 0;
}
