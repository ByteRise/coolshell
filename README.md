# CoolShell

CoolShell is a custom command-line shell program written in C++. It provides various utility commands and features beyond a typical terminal shell.

## Overview

The `coolshell.cpp` file contains the main implementation of the CoolShell program. It utilizes various C++ standard libraries and Windows APIs to create an interactive shell with custom commands.

The `functions.cpp` file contains definitions for utility and helper functions used by the main `coolshell.cpp` program. This separation of concerns keeps the main file clean.

## Features

- **Custom commands:** In addition to executing system commands, CoolShell has various custom commands:

  - `help`: Print available commands
  - `git`: Enter Git shell  
  - `purple`: Enter Purple shell
  - `cd`: Change directory
  - `rm`: Remove file
  - `ls`: List directory contents  
  - `clear`: Clear screen
  - `pwd`: Print working directory
  - `date`: Print date
  - `time`: Print time
  - `hostname`: Print computer name
  - `echo`: Print input text
  - `download`: Download package
  - `wsl`: Toggle WSL mode
  - `hash`: Print file hashes
  - `info`: Print system info

- **WSL mode:** Toggle between Windows and WSL mode for executing system commands.

- **File utilities:** Remove, list, read files with custom commands.

- **System info:** Print info like computer name, date, time, etc. 

- **Input parsing:** Split input into tokens for processing.

- **Directory navigation**: Change current working directory.

- **Package management**: Download packages using `winget` or `apt-get`.

- **Git shell**: Enter dedicated Git shell.

## Code Overview

**`coolshell.cpp`**

- `main()`: Program entry point. Implements shell REPL loop.

- `splitString()`: Split input into tokens.

- `runCommand()`: Execute system command.

- `initWslShell()`: Toggle WSL mode on/off.

- Calls out to `functions.cpp` for other utilities.

**`functions.cpp`**

- `changeDirectory()`: Change current working directory.

- `removeFile()`: Delete a file.

- `listFilesInDirectory()`: Print formatted directory listing.

- `printCurrentPath()`: Print current working directory path. 

- `printCurrentDate()`, `printCurrentTime()`: Print current date and time strings.

- `printComputerName()`: Print computer name.

- `echoText()`: Print input text. 

- `downloadPackage()`: Download package via package manager.

- `printSystemInfo()`: Print system info like OS version, computer name, etc.

- `clearScreen()`: Clear console screen.

- And more...
s
## Build and Run

To build and run CoolShell, ensure you have a C++ compiler and the necessary build tools installed. This program uses the `Windows.h` and `filesystem` libraries, which may require a C++17 compatible compiler.

```
g++ -std=c++17 coolshell.cpp -o coolshell
./coolshell
```

BUT!!! Recommended compile with visual studio native tools.

```
cl /std:c++17 coolshell.cpp
```

## Contributions

Contributions are welcome! If you have ideas for improvements or bug fixes, please feel free to make a pull request or open an issue.

## Roadmap (but its actually a todo)
- [x] Do a roadmap
- [x] Implement purple support
- [ ] Make it cross-platform
- [ ] Add docker-shell
- [x] Add ssh, ftp... support (added by redirecting unknown commands to the regular console)
- [ ] Add `scan` network utility (in progress)
- [ ] Complete the `dc` so that it works for all files

## Disclaimer

This tool is provided as-is, and while it has been tested, the developers are not responsible for any damage or data loss that may occur due to its use.