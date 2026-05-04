# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = 

# Executable name
TARGET = mini_c

# Object files (from your dependencies)
OBJS = read_characters.o lexer.o symbol_table.o parser.o ast_constructor.o ast_traversal.o main.o

# Header dependencies (simplified for clarity)
DEPS = read_characters.h lexer.h parser.h ast_constructor.h ast_traversal.h ast_type.h symbol_table.h

# Default target
all: $(TARGET)

# Link all objects into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile each .c file with explicit dependencies
read_characters.o: read_characters.c read_characters.h
	$(CC) $(CFLAGS) -c $< -o $@

lexer.o: lexer.c lexer.h read_characters.h
	$(CC) $(CFLAGS) -c $< -o $@

parser.o: parser.c parser.h lexer.h ast_constructor.h ast_type.h
	$(CC) $(CFLAGS) -c $< -o $@

ast_constructor.o: ast_constructor.c ast_constructor.h ast_type.h
	$(CC) $(CFLAGS) -c $< -o $@

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) $(CFLAGS) -c $< -o $@

ast_traversal.o: ast_traversal.c ast_traversal.h ast_type.h ast_constructor.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c parser.h
	$(CC) $(CFLAGS) -c $< -o $@

# Test target
test_parser: test_parser.c
	$(CC) $(CFLAGS) $< -o $@

test: $(TARGET) test_parser
	./test_parser

# Cleanup
clean:
	rm -f $(OBJS) $(TARGET) test_parser

.PHONY: all clean