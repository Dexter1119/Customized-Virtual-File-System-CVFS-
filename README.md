

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
* Case-insensitive command recognition using `strcasecmp`.

✅ **File Handling Operations**

* Create, Open, Read, Write, Close, CloseAll, Truncate, and Delete files.
* Support for file permissions (read-only, write-only, read+write).
* File sizes capped at **100 bytes per file** (configurable via `MAXFILESIZE`).

✅ **Additional Functionalities**

* `stat` and `fstat` to view file metadata by name or descriptor.
* `ls` to list all files in a formatted table.
* `man` and `help` for user guidance.
* Error handling with descriptive messages (invalid parameters, permission denied, file not found, etc.).
* `exit` command to gracefully shut down the shell.

---

## 🖥️ CVFS Shell

The project provides a custom **command-line shell** for user interaction:

```text
CVFS : > creat demo.txt 3
File 'demo.txt' created successfully. File Descriptor: 0

CVFS : > write 0
Please enter the data that you want to write into the file: Hello CVFS World!
16 bytes of data written into file successfully.

CVFS : > read 0 10
10 bytes read from file.
Data : Hello CVFS

CVFS : > ls
-------------------------------------------------------------
File Name            Inode Number    File Size    Actual File Size
-------------------------------------------------------------
demo.txt             1               100          16             
-------------------------------------------------------------

CVFS : > close 0
File with descriptor 0 closed successfully.

CVFS : > open demo.txt 1
File 'demo.txt' opened successfully. File Descriptor: 0

CVFS : > fstat 0
----------Statistical Information of File-------------
File Descriptor    : 0
File Name          : demo.txt
File Size On Disk  : 100
Actual File Size   : 16
...

CVFS : > rm demo.txt
File 'demo.txt' deleted successfully.

CVFS : > exit
Thank You for Using CVFS
Deallocating the Resources ...
```

---

## 📖 Supported Commands

| Command                         | Description                                                      |
| ------------------------------- | ---------------------------------------------------------------- |
| `creat <filename> <permission>` | Create a new file with given permission (1=Read/2=Write/3=R+W). |
| `open <filename> <mode>`        | Open an existing file in given mode.                             |
| `close <FD>`                    | Close the file descriptor.                                       |
| `closeall`                      | Close all open files.                                            |
| `write <FD>`                    | Write data into a file (prompts for input).                      |
| `read <FD> <size>`              | Read given size (bytes) from file.                               |
| `ls`                            | List all files with details (name, inode, size, actual size).    |
| `stat <filename>`               | Display metadata of file by name.                                |
| `fstat <FD>`                    | Display metadata of file by descriptor.                          |
| `truncate <filename>`           | Remove file content but keep metadata.                           |
| `rm <filename>`                 | Delete a file.                                                   |
| `man <command>`                 | Manual page for a specific command.                              |
| `help`                          | Display list of supported commands.                              |
| `clear`                         | Clear the terminal screen.                                       |
| `exit`                          | Exit the CVFS shell.                                             |

---

## 🛠️ Build Instructions

```bash
g++ -o cvfs CVFS.cpp
./cvfs
```

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

✔️ **Memory Management** – `malloc`, `free`, and careful handling of buffers.

✔️ **User-Friendly Errors** – Proper messages for invalid operations.

✔️ **Shell Simulation** – Implemented with infinite loop, command parsing, and user prompts.

---

## 🔮 Future Enhancements

* Add **directory support** (currently flat file system).
* Persistent storage (saving CVFS state to a disk file).
* Multi-user support with authentication.
* Seek support (lseek) to reposition read/write offsets.

---

## 👨‍💻 Author

**Pradhumnya Changdev Kalsait**
📅 *September 2025*
🎯 Built for learning operating system & file system fundamentals.

