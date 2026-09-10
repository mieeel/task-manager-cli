# 📝 Task Manager CLI

A lightweight, high-performance command-line Task Manager built in Modern C++ (C++17). Designed for speed, persistent local storage, custom color outputs, progress tracking, scheduled events, tags, subtasks, and priority handling.

<p align="left">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue.svg" alt="C++ Version" />
  <img src="https://img.shields.io/badge/License-MIT-green.svg" alt="License" />
</p>

---

## ✨ Features

- ⚡ **Lightning Fast:** Native C++ CLI with zero heavy dependencies.
- 📅 **Events & Deadlines:** Assign due dates/times (`due:YYYY-MM-DD` or `due:YYYY-MM-DDTHH:MM`) and view them via `task events`.
- 🎨 **ANSI Color Output:** Colored status indicators, tag badges, priority highlights, and event indicators.
- 📊 **Progress & Stats:** Built-in visual progress bar and `task stats` completion analytics.
- 🎯 **Priority Management:** Support for HIGH, MEDIUM, and LOW priorities with auto-sorting and filtering.
- 🏷️ **Tags & Categorization:** Organize tasks using custom tags (e.g., `@work`, `@study`).
- 🌿 **Subtasks Support:** Nest subtasks under parent task IDs (tree visualization).
- ⚡ **Batch Operations:** Complete or remove multiple task IDs in a single command (`task done 1 2 3`).
- 🔍 **Search & Edit:** Fast keyword search and live task editing capabilities.
- 💾 **Persistent Storage:** Saves tasks automatically to `~/.tasks.txt` across terminal sessions.
- 🌐 **Global Access:** Install as a system-wide CLI utility (`task`).

---

## 🛠️ Building & Installation

### Prerequisites
- GCC / g++ (with C++17 support) or Clang.

### 1. Build from source
git clone https://github.com/mieeel/task-manager-cli.git
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
| :--- | :--- |
| `task` / `task ls` | List all **pending** tasks (Sorted by Priority & Date) |
| `task ls --all` | List **all** tasks (Pending + Completed) |
| `task ls --done` | List **completed** tasks |
| `task events` | List **only** scheduled events and tasks with due dates |
| `task ls --prio <h\|m\|l>` | List pending tasks filtered by priority |
| `task ls @tag` | List pending tasks filtered by a specific tag |
| `task search <keyword>` | Search tasks by title keyword |
| `task stats` | View detailed completion statistics and metrics |
| `task add "Meeting" due:2026-09-15T14:30 @work` | Add a scheduled event/task |
| `task add "Title" @tag p:high` | Add a task with tags and priority |
| `task add "Title" sub:<ID>` | Add a subtask linked to a parent task ID |
| `task edit <ID> "Title" due:2026-09-20` | Edit title, priority, tags, or due date |
| `task done <ID1> <ID2> ...` | Mark one or multiple tasks as completed |
| `task rm <ID1> <ID2> ...` | Permanently delete one or multiple tasks |
| `task clear` | Remove all completed tasks |
| `task --help` | Display help menu |

---

## 📁 Project Structure

.
├── Colors.hpp         # ANSI Escape sequences for terminal formatting
├── Task.hpp           # Task entity, subtasks, tags, due dates, and print formatting
├── TaskManager.hpp    # Task collection management, event sorting, tree rendering, and statistics
├── StorageManager.hpp # Local disk persistence (fstream parsing)
└── main.cpp           # CLI argument parsing, flags routing, and entrypoint

---

## 📄 License

Distributed under the MIT License. See `LICENSE` for more information.