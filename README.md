# System Monitor

A lightweight, real-time system monitoring tool written in pure C for Linux systems. Displays CPU usage, memory statistics, and process information in an interactive terminal interface.

![System Monitor Screenshot](screenshot.png)

<!-- Add a screenshot of your application -->

## Features

- **Real-time Process Monitoring**

  - View all running processes with PID, command, and CPU usage
  - Scrollable process list
  - Kill processes directly from the interface
  - Sort processes by various metrics

- **System Metrics**

  - Total and available memory
  - Buffer and cache information
  - Memory usage percentage with visual indicator
  - CPU information (model, cores, frequency)
  - Process count

- **Performance Optimized**
  - Efficient file descriptor management
  - Minimal memory footprint
  - Low CPU overhead

## Prerequisites

- Linux operating system
- GCC compiler
- Make build system
- Access to `/proc` filesystem

## Building

```bash
# Clone the repository
git clone https://github.com/yourusername/system-monitor.git
cd system-monitor

# Build the project
make

# Run
./bin/system-monitor
```

## Usage

### Basic Controls

- **↑/↓ Arrow Keys**: Scroll through process list
- **k**: Kill selected process
- **s**: Sort processes (cycles through: CPU, Memory, PID, Name)
- **q**: Quit application

### Command Line Options

```bash
./system-monitor [OPTIONS]

Options:
  -r, --refresh <ms>    Set refresh rate in milliseconds (default: 1000)
  -h, --help           Display this help message
  -v, --version        Display version information
```

## Project Structure

```
system-monitor/
├── include/
│   ├── third_party/      # External dependencies
│   ├── core/             # Core functionality
│   │   └── processes/    # Process management module
│   │       ├── structs.h
│   │       ├── helpers.h
│   │       └── index.h   # Public API
│   ├── ui/               # User interface modules
│   ├── context.h         # Application context
│   └── utils.h           # Utility functions
├── src/
│   ├── core/
│   │   └── processes/
│   │       ├── helpers.c # Internal implementation
│   │       └── index.c   # Public interface
│   ├── ui/               # UI implementation
│   ├── main.c            # Entry point
│   ├── context.c         # Context management
│   └── utils.c           # Utility implementations
├── Makefile
└── README.md
```

### Architecture

This project follows a **feature-based modular architecture**:

- Each feature (e.g., `processes/`) is self-contained in its own directory
- **`index.h/c`**: Public API that other modules interact with
- **`helpers.h/c`**: Internal implementation details
- **`structs.h`**: Data structures (private to the module)

This design provides clear module boundaries and prevents tight coupling between components.

## Technical Details

### Data Sources

- **Process Information**: `/proc/[pid]/stat`, `/proc/[pid]/status`
- **Memory Statistics**: `/proc/meminfo`
- **CPU Information**: `/proc/cpuinfo`, `/proc/stat`

### Performance Optimizations

1. **File Descriptor Caching**: System-wide files like `/proc/meminfo` are opened once and reused
2. **Efficient Parsing**: Custom parsers optimized for `/proc` filesystem format
3. **Minimal Allocations**: Pre-allocated buffers where possible

## Roadmap

- [x] Basic process listing
- [x] Memory statistics
- [x] CPU information
- [x] Process killing
- [x] Sorting functionality
- [x] Scrolling interface
- [ ] Per-core CPU usage
- [ ] CPU temperature monitoring
- [ ] Additional memory metrics (swap, cached, shared)
- [ ] Process tree view
- [ ] Search/filter processes
- [ ] Configuration file support
- [ ] Color themes
- [ ] Run Options

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by tools like `htop`, `top`, and `btop`
- Built as a learning project to understand Linux systems programming

## Author

**Your Name**

- GitHub: [@dreamInBits01](https://github.com/DreamInBits01)
- Email: mohammed.abdulaziz.business@gmail.com

## FAQ

**Q: Why C instead of a higher-level language?**  
A: C provides direct access to system calls and `/proc` filesystem with minimal overhead, making it ideal for system monitoring tools.

**Q: Does this work on macOS or Windows?**  
A: No, this tool is Linux-specific as it relies on the `/proc` filesystem. However, similar implementations could be created for other platforms using their respective APIs.

**Q: How does this compare to htop?**  
A: This is a learning project and much simpler than htop. It focuses on core functionality and clean architecture rather than feature completeness.

---

**Star ⭐ this repository if you find it helpful!**
