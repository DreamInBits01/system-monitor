CC = gcc
CFLAGS = -g -Wall -Wextra -Iinclude
LIBS = -lncurses -lpanel -pthread

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
INCLUDE_DIR = include
DEPS_DIR = $(INCLUDE_DIR)/third_party

# Installation directories
PREFIX ?= /usr/local
INSTALL_BIN = $(PREFIX)/bin

# Target executable
TARGET = $(BIN_DIR)/index
INSTALL_NAME = warden

# Dependency: uthash
UTHASH_DIR = $(DEPS_DIR)/uthash
UTHASH_URL = https://github.com/troydhanson/uthash.git

# Source files
MAIN_SRC = $(SRC_DIR)/main.c
CONTEXT_SRC = $(SRC_DIR)/context/index.c \
              $(SRC_DIR)/context/helpers.c 
UTILS_SRC = $(SRC_DIR)/utils.c
UI_SRC = $(SRC_DIR)/ui/init.c \
         $(SRC_DIR)/ui/render.c \
         $(SRC_DIR)/ui/interactivity.c
CORE_SRC = $(SRC_DIR)/core/cpu/index.c \
           $(SRC_DIR)/core/cpu/helpers.c \
           $(SRC_DIR)/core/memory/index.c \
           $(SRC_DIR)/core/memory/helpers.c \
           $(SRC_DIR)/core/processes/index.c \
           $(SRC_DIR)/core/processes/helpers.c \
           $(SRC_DIR)/core/system/index.c 

# All source files
SOURCES = $(MAIN_SRC) $(CONTEXT_SRC) $(UTILS_SRC) $(UI_SRC) $(CORE_SRC)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# Force uthash to be downloaded before compiling any object files
$(OBJECTS): | $(UTHASH_DIR)
# Default target
all: dependencies directories $(TARGET)

# Download and setup dependencies
dependencies: $(UTHASH_DIR)

$(UTHASH_DIR):
	@echo "Downloading uthash..."
	@mkdir -p $(DEPS_DIR)
	@if command -v git >/dev/null 2>&1; then \
		git clone --depth 1 $(UTHASH_URL) $(UTHASH_DIR); \
	else \
		echo "Error: git is required to download dependencies"; \
		exit 1; \
	fi
	@echo "uthash downloaded successfully"

# Create necessary directories
directories:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/ui
	@mkdir -p $(BUILD_DIR)/core
	@mkdir -p $(BUILD_DIR)/context

# Link object files to create executable
# $(TARGET): $(OBJECTS)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
# 	@echo "Build complete: $(TARGET)"
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "Build complete: $(TARGET)"
# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Install the program
install: $(TARGET)
	@echo "Installing $(INSTALL_NAME) to $(INSTALL_BIN)"
	@mkdir -p $(INSTALL_BIN)
	install -m 755 $(TARGET) $(INSTALL_BIN)/$(INSTALL_NAME)
	@echo "Installation complete. Run with: $(INSTALL_NAME)"

# Uninstall the program
uninstall:
	@echo "Removing $(INSTALL_BIN)/$(INSTALL_NAME)"
	rm -f $(INSTALL_BIN)/$(INSTALL_NAME)
	@echo "Uninstall complete"

# Clean build artifacts (keep dependencies)
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Clean everything including dependencies
distclean: clean
	rm -rf $(DEPS_DIR)
	@echo "Deep clean complete"

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Check dependencies
check-deps:
	@echo "Checking for required dependencies..."
	@command -v $(CC) >/dev/null 2>&1 || { echo "Error: gcc not found"; exit 1; }
	@command -v git >/dev/null 2>&1 || { echo "Error: git not found"; exit 1; }
	@pkg-config --exists ncurses 2>/dev/null || { echo "Warning: ncurses not found via pkg-config"; }
	@echo "Dependencies check complete"

# Phony targets
.PHONY: all clean distclean rebuild run directories install uninstall check-deps dependencies