# # Simple Makefile for learning

# # Variables
# CC = gcc
# CFLAGS = -Wall -Iinclude -pthread

# SRC_DIR = src
# BUILD_DIR = build
# BIN_DIR = bin
# INCLUDE_DIR = include
# # Default target - builds the program
# all: index

# # Build the main program
# # Target: index
# # Dependencies: all .c files in src/
# # Command: compile everything together
# index: src/main.c
# 	$(CC) $(CFLAGS) -o bin/index src/utils.c src/main.c src/routines.c src/memory.c src/processes.c  src/cpu.c  -lncurses -pthread

# # Clean up - removes built files
# clean:
# 	rm -f index

# # Run the program
# run: index
# 	./bin/index

# # Phony targets (don't create files with these names)
# .PHONY: all clean run

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -pthread
LIBS = -lncurses -pthread

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include

# Target executable
TARGET = $(BIN_DIR)/index

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
CONTEXT_SRC = $(SRC_DIR)/context.c
UTILS_SRC = $(SRC_DIR)/utils.c

UI_SRC = $(SRC_DIR)/ui/init.c \
         $(SRC_DIR)/ui/render.c \
         $(SRC_DIR)/ui/interactivity.c

CORE_SRC = $(SRC_DIR)/core/cpu.c \
           $(SRC_DIR)/core/memory.c \
           $(SRC_DIR)/core/processes.c

# All source files
SOURCES = $(MAIN_SRC) $(CONTEXT_SRC) $(UTILS_SRC) $(UI_SRC) $(CORE_SRC)

# Object files (replace src/ with build/ and .c with .o)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/ui
	@mkdir -p $(BUILD_DIR)/core

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean rebuild run directories