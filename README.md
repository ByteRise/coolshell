# CoolShell

CoolShell is a custom shell utility designed to enhance the command-line experience in Windows environments. It also offers functionality to switch to WSL (Windows Subsystem for Linux) mode for a seamless transition between Windows and Linux commands.

## Features

- **WSL Mode Toggle**: Switch between Windows and WSL mode to execute commands in the desired environment.
- **Git Shell**: Enter a dedicated shell for executing Git commands.
- **Purple Shell**: Automatically adds `purple --command` before the command
- **Download Packages**: Supports package installation using `apt-get` in WSL mode or `winget` in Windows mode (note: `winget` support is a TODO).
- **File Operations**: Remove files, list files in the current directory, and display file sizes in a human-readable format.
- **System Information**: Print the current system's computer name, path, date, and time.
- **Text Output**: Echo text back to the console.
- **Directory Navigation**: Change the current working directory.
- **Clear Screen**: Clear the console output.
- **Command Help**: Display a list of available commands.

## Usage

Run `coolshell.cpp` in your preferred C++ environment. Once started, you will see a prompt indicating the mode you are in (`coolshell@main` for Windows or `coolshell@wsl` for WSL mode and `coolshell@git` for git mode). You can enter commands directly at the prompt.

### Available Commands

- `help`: Prints the help message with available commands.
- `git`: Initiates a git-shell.
- `cd <path>`: Changes the current directory to the specified path.
- `rm <filename>`: Removes the specified file.
- `ls`: Lists files in the current directory with icons and sizes.
- `clear`: Clears the console screen.
- `rf`: Reads file context
- `pwd`: Prints the current working directory path.
- `dc`: Deletes comments in code (only cpp files for now)
- `date`: Prints the current date.
- `time`: Prints the current time.
- `hostname`: Prints the name of the computer.
- `echo <text>`: Prints the provided text.
- `download <package>`: Downloads the specified package using the appropriate package manager.
- `wsl`: Toggles WSL mode on or off.
- `purple`: Toggles purple mode on or off.
- `info`: Prints info of computer
- `exit`: Exits the shell.

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