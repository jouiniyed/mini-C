# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
LDFLAGS =

# Directories
SRCDIR = src
INCDIR = include

# Executable name
TARGET = mini_c

# Object files
OBJS = read_characters.o lexer.o symbol_table.o parser.o ast_constructor.o ast_traversal.o main.o

# Default target
all: $(TARGET)

# Link all objects into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compile each .c file with explicit dependencies
read_characters.o: $(SRCDIR)/read_characters.c $(INCDIR)/read_characters.h
	$(CC) $(CFLAGS) -c $< -o $@

lexer.o: $(SRCDIR)/lexer.c $(INCDIR)/lexer.h $(INCDIR)/read_characters.h
	$(CC) $(CFLAGS) -c $< -o $@

parser.o: $(SRCDIR)/parser.c $(INCDIR)/parser.h $(INCDIR)/lexer.h $(INCDIR)/ast_constructor.h $(INCDIR)/ast_type.h
	$(CC) $(CFLAGS) -c $< -o $@

ast_constructor.o: $(SRCDIR)/ast_constructor.c $(INCDIR)/ast_constructor.h $(INCDIR)/ast_type.h
	$(CC) $(CFLAGS) -c $< -o $@

symbol_table.o: $(SRCDIR)/symbol_table.c $(INCDIR)/symbol_table.h
	$(CC) $(CFLAGS) -c $< -o $@

ast_traversal.o: $(SRCDIR)/ast_traversal.c $(INCDIR)/ast_traversal.h $(INCDIR)/ast_type.h $(INCDIR)/ast_constructor.h
	$(CC) $(CFLAGS) -c $< -o $@

main.o: $(SRCDIR)/main.c $(INCDIR)/parser.h
	$(CC) $(CFLAGS) -c $< -o $@

# Test target
test_parser: $(SRCDIR)/test_parser.c
	$(CC) $(CFLAGS) $< -o $@

test: $(TARGET) test_parser
	./test_parser

# Cleanup
clean:
	rm -f $(OBJS) $(TARGET) test_parser

.PHONY: all clean
