#feito por Chat Gepeto
# Compiler
CC = mpicc

# Compiler flags
CFLAGS = -O3 -Wall -Isrc -pthread -lm

# Source files
SRC_DIR = src
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=%.o)

# Output executable
OUTPUT = knn-mpi+pth

# Build target
all: $(OUTPUT) clean

# Main target
$(OUTPUT): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compile source files to object files
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	rm -f $(OBJ_FILES)

purge:
	rm -f $(OBJ_FILES) $(OUTPUT)

.PHONY: all clean
