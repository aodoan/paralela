#feito por Chat Gepeto
# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Isrc -pthread

# Source files
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=%.o)

# Output executable
OUTPUT = acharKMenores

# Build target
all: $(OUTPUT)

# Main target
$(OUTPUT): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compile source files to object files
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -f $(OUTPUT) $(OBJ_FILES)

.PHONY: all clean
