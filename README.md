# System Monitor

A terminal-based system monitoring tool written in C that provides real-time insights into system resources and processes.

![System Monitor Screenshot](screenshot.png)

<!-- Add a screenshot of your application -->

## Features

### 🖥️ Resource Monitoring

- **Memory Usage**

  - Total and free memory display
  - Memory usage percentage
  - Visual memory usage bar
  - Buffer information

- **CPU Statistics**

  - Real-time CPU usage
  - Model information
  - Core details
  - Dynamic updates

- **Process Management**
  - Live process list
  - Process navigation
  - Process details (PID, name, status)
  - Interactive process selection

### 🎨 User Interface

- **Multi-window Layout**

  - Separate windows for different metrics
  - Clean border separation
  - Organized information display

- **Interactive Features**
  - Keyboard navigation
  - Scrollable process list
  - Window focus management
  - Color-coded information

### 🔧 Technical Features

- Multi-threaded architecture
- Real-time updates
- Thread-safe operations
- ncurses-based UI
- Efficient process monitoring
- Linux system integration

## Architecture

### Components

- **Core Modules**
  - Memory monitoring
  - CPU statistics
  - Process management
- **UI Components**

  - Window management
  - User input handling
  - Display rendering

- **Context Management**
  - Thread synchronization
  - State management
  - Resource cleanup

## Building

### Prerequisites

- GCC compiler
- ncurses library
- Linux-based system
- Make utility

### Compilation

```bash
make clean
make
```

### Running

```bash
./bin/index
```

## Usage

### Navigation

- Arrow keys: Navigate process list
- Tab: Switch between windows
- Q: Quit application

### Windows

- Memory information (top-left)
- CPU statistics (top-right)
- Process list (bottom)

## Implementation Details

The project uses:

- ncurses for terminal UI
- POSIX threads for concurrency
- Linux proc filesystem for system data
- uthash for hash table operations

## Acknowledgment

This project is for learning purposes, it is inspired by htop & btop

## License

[Your chosen license]

## Author

[Mohammed Abdulaziz]
