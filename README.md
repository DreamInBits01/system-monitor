# Warden

**Warden** is a high-performance, lightweight terminal task manager for Linux, built using **C** and **Ncurses**. It provides real-time system monitoring and process management with an extremely low resource footprint.

![System Monitor Screenshot](screenshot.png)

---

## 🚀 Key Features

- **CPU Monitoring:** Real-time visualization of overall CPU usage and individual core performance.
- **Memory Stats:** Detailed breakdown of Total, Available, Free, and Buffered memory.
- **Process Viewer:**
  - View detailed info: `PID`, `Threads`, `CPU%`, `Name`, and `User`.
  - **Kill Processes:** Terminate unwanted processes directly from the UI.
- **Sorting:** Support for default sorting and sorting by CPU usage.

---

## ⚡ Performance & Optimization

Warden is designed to be lighter than the processes it monitors.

- **Resource Usage:** Consumes only **0.1% - 0.6%** of CPU.
- **Caching Strategy:**
  - **File Descriptors:** Heavily used descriptors are cached during initialization to reduce syscall overhead.
  - **Data Structures:** Uses **hash tables** (via `uthash`) for O(1) lookups.
    - _Processes Table:_ Stores process positions.
    - _Y-to-PID Map:_ Maps the UI cursor's vertical position ($y$) directly to the corresponding process entry for instant interaction.

---

## 🏗️ Architecture

The codebase follows a **hybrid MVC and Feature-based** architecture.

### Structure

- **Modular UI:** Each UI block is treated as a standalone feature containing its own `index` and `helpers` file.
- **Main Components:**
  1.  **Context:** Manages application state.
  2.  **UI:** Handles rendering and layout.
  3.  **Core:** Manages system data fetching and logic.

### Concurrency

Warden is **multi-threaded** to ensure the interface remains buttery smooth even under load.

- **Threads:** Separate threads for **Rendering** and **Interactivity**.
- **Sync:** Uses a "Ping-Pong" synchronization style to coordinate safely between the UI and data threads.

---

## 🛠️ Installation

### Dependencies

- **Linux**
- **Ncurses** library
- **uthash** (Macro-based hash table for C structures)

### Build Instructions

Warden includes a `Makefile` for automated building and installation.

```bash
# Clone the repository
git clone https://github.com/dreaminbits01/warden.git
cd warden

# Compile and Install
make
sudo make install
```

### Usage

Run the application from your terminal:

```bash
warden
```

---

## 🔮 Roadmap

- [ ] **Memory Sorting:** Ability to sort processes by RAM usage.
- [ ] **CLI Options:** Support for command-line arguments and flags.

---
