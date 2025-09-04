

# 📂 CVFS – Custom Virtual File System

A **Custom Virtual File System (CVFS)** implemented in **C++** that simulates basic file system operations inside a shell-like interface.
It provides an educational, simplified model of a UNIX-like file system with file handling, metadata management, and user interaction.

---

## ✨ Features

✅ **Virtual File System Simulation**

* Implements core structures: Boot Block, Super Block, Inode Table, File Table, and User File Descriptor Table.

✅ **Interactive Shell**

* Custom **CVFS Shell** with built-in commands.
* Command parsing similar to a UNIX shell (`creat`, `ls`, `read`, `write`, etc.).
* Case-insensitive command recognition using `stricmp`.

✅ **File Handling Operations**

* Create, Open, Read, Write, Close, and Delete files.
* Support for file permissions (read, write, read+write).
* File sizes capped at **1 KB per file**.

✅ **Additional Functionalities**

* `stat` and `fstat` to view file metadata.
* `ls` to list all files.
* `man` and `help` for user guidance.
* Error handling with descriptive messages (invalid parameters, permission denied, file not found, etc.).
* `exit` command to gracefully shut down the shell.

---

## 🖥️ CVFS Shell

The project provides a custom **command-line shell** for user interaction:

```text
Marvellous CVFS : > creat demo.txt Write
File Successfully Created with File Descriptor : 3

Marvellous CVFS : > write 3
Enter the Data : Hello CVFS World!
Successfully written 18 bytes into the file

Marvellous CVFS : > read 3 10
10 Bytes Successfully Read from File
Data : Hello CVFS

Marvellous CVFS : > ls
-------------------------------------------------------------
File Name       Inode number    File Size    Actual File Size
-------------------------------------------------------------
demo.txt        3               1024         18
-------------------------------------------------------------

Marvellous CVFS : > exit
Terminating the Marvellous Virtual File System...
```

---

## 📖 Supported Commands

| Command                         | Description                                                      |
| ------------------------------- | ---------------------------------------------------------------- |
| `creat <filename> <permission>` | Create a new file with given permission (Read/Write/Read+Write). |
| `open <filename> <mode>`        | Open an existing file in given mode.                             |
| `close <FD>`                    | Close the file descriptor.                                       |
| `closeall`                      | Close all open files.                                            |
| `write <FD>`                    | Write data into a file.                                          |
| `read <FD> <size>`              | Read given size (bytes) from file.                               |
| `ls`                            | List all files with details.                                     |
| `stat <filename>`               | Display metadata of file by name.                                |
| `fstat <FD>`                    | Display metadata of file by descriptor.                          |
| `truncate <filename>`           | Remove file content but keep metadata.                           |
| `rm <filename>`                 | Delete a file.                                                   |
| `man <command>`                 | Manual page for a specific command.                              |
| `help`                          | Display list of supported commands.                              |
| `clear`                         | Clear the terminal screen.                                       |
| `exit`                          | Exit the CVFS shell.                                             |

---

## 🛠️ Coding Conventions Followed

✔️ **Header Documentation for Functions** – Each function contains:

* Function name
* Description
* Author
* Date
* Parameters & I/O details

✔️ **Error Codes Defined** – All error states handled with constants (`ERR_FILE_NOT_EXISTS`, `ERR_PERMISSION_DENIED`, etc.).

✔️ **Consistent Naming** –

* Structures in **UpperCamelCase** (`INODE`, `SUPERBLOCK`).
* Constants in **UPPERCASE**.
* Variables in **lowercase\_with\_underscores**.

✔️ **Memory Management** – `malloc`, `free`, and careful handling of buffers (fixed in `read`).

✔️ **User-Friendly Errors** – Proper messages for invalid operations.

✔️ **Shell Simulation** – Implemented with infinite loop, command parsing, and user prompts.

---

## 🔮 Future Enhancements

* Add **directory support** (currently flat file system).
* Support for **append mode** in `write`.
* Persistent storage (saving CVFS state to a file).
* Multi-user support with authentication.

---

## 👨‍💻 Author

**Pradhumnya Changdev Kalsait**
📅 *September 2025*
🎯 Built for learning operating system & file system fundamentals.

