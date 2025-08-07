# Simple Makefile for learning

# Variables
CC = gcc
CFLAGS = -Wall -Iinclude

# Default target - builds the program
all: index

# Build the main program
# Target: index
# Dependencies: all .c files in src/
# Command: compile everything together
index: src/main.c 
	$(CC) $(CFLAGS) -o bin/index src/main.c

# Clean up - removes built files
clean:
	rm -f index

# Run the program
run: index
	./bin/index

# Phony targets (don't create files with these names)
.PHONY: all clean run