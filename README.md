# mini_C Calculator

![mini_C](mini_c.jpg)

A recursive-descent parser and interpreter for a mini-C language subset. It reads a source file, builds an Abstract Syntax Tree (AST), evaluates assignments using a symbol table, and prints the resulting tree.

---

## Project Structure

| File | Role |
|------|------|
| `read_characters.c / .h` | Character-level reader: opens a file, tracks line/column, advances one character at a time |
| `lexer.c / .h` | Tokenizer: groups characters into tokens (integers, identifiers, operators, keywords) |
| `ast_type.h` | Enum definitions for AST node types and operator types |
| `ast_constructor.c / .h` | AST node allocation: `create_value`, `create_assignment`, `create_condition`, etc. |
| `ast_traversal.c / .h` | AST traversal: `print_ast` for display, `bool_evaluation` / `evaluation` for value computation |
| `symbol_table.c / .h` | Linked-list symbol table: stores identifier names and their integer values |
| `parser.c / .h` | Recursive-descent parser: converts the token stream into an AST, evaluates expressions at parse time |
| `main.c` | Entry point: calls `parse()` then `print_ast()` |
| `test_parser.c` | Standalone test suite: 10 valid and 10 invalid input cases |
| `inputbis.txt` | Default input file (hardcoded in `main.c`) |
| `Makefile` | Build rules for all targets |

---

## Grammar

```
program         → seq_inst

seq_inst        → assignment seq_inst
                | if_part seq_inst
                | while_loop seq_inst
                | ε

assignment      → IDENTIFIER = boolean_expression ;

if_part         → if ( boolean_expression ) { seq_inst } else_part

else_part       → else { seq_inst }
                | ε

while_loop      → while ( boolean_expression ) { seq_inst }

boolean_expression → bool_term ( || bool_term )*

bool_term       → primary ( && primary )*

primary         → ! primary
                | relation

relation        → gae ( comparison_op gae )?

comparison_op   → == | != | < | > | <= | >=

gae             → term ( ( + | - ) term )*

term            → factor ( ( * | / ) factor )*

factor          → INTEGER
                | ( boolean_expression )
                | IDENTIFIER
```

Identifiers are resolved to their integer value at parse time via the symbol table. Using an undeclared variable is a fatal error.

---

## How to Compile

```bash
make
```

This builds the `mini_c` executable. To also compile the test binary:

```bash
make test_parser
```

To clean all build artifacts:

```bash
make clean
```

---

## How to Run

Edit `inputbis.txt` with your mini-C program, then:

```bash
./mini_c
```

### Example input (`inputbis.txt`)

```c
x = 3;
y = 6;
z = 1;
e = ((x + y) == 9) && (z || (y < 2));
```

### Example output

```
SYNTAX IS OK

Symbol table:
    e = 1 (bool: 1)
    z = 1 (bool: 1)
    y = 6 (bool: 1)
    x = 3 (bool: 1)
SEQ_INST
  ASSIGNMENT
    IDENTIFIER(x)
    VALUE(3)
  SEQ_INST
    ...
```

---

## How to Run the Tests

The test suite overwrites `inputbis.txt` with each test case and runs `./mini_c` as a subprocess.

```bash
make test
```

Or manually:

```bash
make
make test_parser
./test_parser
```

---

## Known Limitations

- The input filename is hardcoded as `inputbis.txt` in `main.c`.
- Identifiers are resolved to their integer value at parse time, not at runtime. This means `while` loops do not execute repeatedly — the body is parsed once and the loop condition is checked once.
- There is no `print` statement or I/O beyond assignments.
- Only integer arithmetic is supported; there are no floating-point values.
- Division by zero causes a runtime error via `exit(1)`.
- The `&&` and `||` operators do not short-circuit during evaluation.
