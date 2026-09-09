# 📝 Task Manager CLI

A lightweight, high-performance command-line Task Manager built in Modern C++ (C++17). Designed for speed, persistent local storage, custom color outputs, and priority handling.

<p align="left">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++ Version" />
  <img src="https://img.shields.io/badge/License-MIT-green.svg" alt="License" />
</p>

---

## ✨ Features

* ⚡ **Lightning Fast:** Native C++ CLI with zero heavy dependencies.
* 🎨 **ANSI Color Output:** Colored status indicators and priority badges.
* 🎯 **Priority Management:** Support for HIGH, MEDIUM, and LOW priorities with auto-sorting.
* 💾 **Persistent Storage:** Saves tasks automatically to `~/.tasks.txt` across terminal sessions.
* 🌐 **Global Access:** Install as a system-wide CLI utility (`task`).

---

## 🛠️ Building & Installation

### Prerequisites

* GCC / g++ (with C++17 support) or Clang.

### 1. Build from source

git clone [https://github.com/mieeel/task-manager-cli.git](https://www.google.com/search?q=https://github.com/mieeel/task-manager-cli.git)
cd task-manager-cli
g++ -std=c++17 main.cpp -o task

### 2. Install Globally

#### Linux / macOS

sudo mv task /usr/local/bin/

#### Windows (PowerShell)

1. Move `task.exe` to a permanent folder (e.g., `C:\Tools\TaskCLI\`).
2. Add `C:\Tools\TaskCLI\` to your System Environment Variables (`PATH`).

---

## 🚀 Usage

| Command | Description |
| --- | --- |
| `task` / `task ls` | List all **pending** tasks (Sorted by Priority) |
| `task ls --all` | List **all** tasks (Pending + Completed) |
| `task ls --done` | List **completed** tasks |
| `task search <keyword>` | Search tasks by title keyword |
| `task add "Title"` | Add a task with default **Medium** priority |
| `task add "Title" p:high` | Add a task with **High**, **Med**, or **Low** priority |
| `task done <ID>` | Mark a task as completed (e.g., `task done 1` or `task done 001`) |
| `task rm <ID>` | Permanently delete a task |
| `task clear` | Remove all completed tasks |
| `task --help` | Display help menu |

---

## 📁 Project Structure

```
.
├── Colors.hpp         # ANSI Escape sequences for terminal formatting
├── Task.hpp           # Task entity, priority logic, and print formatting
├── TaskManager.hpp    # Task collection management and sorting algorithms
├── StorageManager.hpp # Local disk persistence (fstream parsing)
└── main.cpp           # CLI argument parsing and entrypoint

```

---

## 📄 License

Distributed under the MIT License. See `LICENSE` for more information.
