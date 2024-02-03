#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <lmcons.h>

using namespace std;

bool isWslMode = false;

void initWslShell() {
    isWslMode = !isWslMode; // Переключает состояние WSL-режима
    cout << "WSL Shell " << (isWslMode ? "activated." : "deactivated.") << endl;
}

bool runCommand(const string& cmd) {
    string fullCmd = isWslMode ? "wsl " + cmd : cmd;
    int result = system(fullCmd.c_str());
    if (result != 0) {
        cerr << "Command failed: " << fullCmd << endl;
        return false;
    }
    return true;
}

bool checkPurpleExeExists() {
    string exeName = "purple.exe";
    string pathEnv;
    char* buffer;
    size_t len;
    _dupenv_s(&buffer, &len, "PATH");
    if (buffer != NULL) {
        pathEnv = buffer;
        free(buffer);
    }

    string delimiter = ";";

    size_t pos = 0;
    string path;
    while ((pos = pathEnv.find(delimiter)) != string::npos) {
        path = pathEnv.substr(0, pos);
        pathEnv.erase(0, pos + delimiter.length());

        filesystem::path exePath = path + "\\" + exeName;
        if (filesystem::exists(exePath)) {
            cout << "DEBUG: The file purple.exe exists in the system's PATH." << endl;
            return true;
        }
    }

    std::cout << "DEBUG: The file purple.exe does not exist in the system's PATH. Starting downloading..." << std::endl;

	string downloadUrl = "https://github.com/ByteRise/purple/releases/download/v1.4/purplepipmanager.exe";
    
    // downloading process
    std::filesystem::path directory = "C:\\purplepipmanager";
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directories(directory);
    }

    string command = "curl -L " + downloadUrl + " -o " + directory.string() + "\\purple.exe";
    std::cout << "Executing command: " << command << std::endl;

    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to download purplepipmanager.exe. Curl command failed. Trying to download it into Downloads..." << std::endl;
    }

    // downloading process 2
    string command2 = "curl -L " + downloadUrl + " -o " + directory.string() + "\\purple.exe";
    std::cout << "Executing command: " << command2 << std::endl;

    int result2 = system(command2.c_str());
    if (result2 != 0) {
        std::cerr << "Failed to download purplepipmanager.exe. Curl command failed. Closing..." << std::endl;
        return false;
    }

	cout << "Downloaded purplepipmanager.exe successfully. Its in C:\\purplepipmanager directory (or Downloads), add its in path or copy file purple.exe to directory, that exists in path" << endl;
    return true;
}

void runGitShell() {
    string gitCmd;
    cout << "Entering git-shell mode. Type 'exit' to leave." << endl;
    while (true) {
        cout << "coolshell@git" << "> ";
        getline(cin, gitCmd);

        if (gitCmd == "exit") {
            break;
		}
		else if (gitCmd == "help") {
			cout << "Commands:" << endl;
			cout << "exit - Exit git-shell mode" << endl;
			cout << "help - Show this help message" << endl;
		}

        runCommand("git " + gitCmd);
    }
}

void runpurpleShell() {
    string purplecmd;
    cout << "Entering purple-shell mode. Type 'exit' to leave." << endl;
    // проверка существует ли файл purple.exe в path
	bool isexist = checkPurpleExeExists();
	if (isexist) {
        while (true) {
            cout << "coolshell@purple" << "> ";
            getline(cin, purplecmd);

            if (purplecmd == "exit") {
                break;
            }
            else if (purplecmd == "help") {
                cout << "Commands:" << endl;
                cout << "exit - Exit purple-shell mode" << endl;
                cout << "help - Show this help message" << endl;
            }

            bool result = runCommand("purple --command" + purplecmd); // !!
			if (!result) {
				runCommand("C:\\Users\\%USERNAME%\\Downloads\\purple.exe --command " + purplecmd);
			}
        }
	}
}

void downloadPackage(const string& packageName) {
    if (isWslMode) {
        cout << "Using apt-get to download " << packageName << "..." << endl;
        runCommand("sudo apt-get install " + packageName);
    }
    else {
        cout << "Using winget to download " << packageName << "..." << endl;
		runCommand("winget install " + packageName);
    }
}

void printComputerName() {
    if (isWslMode) {
        runCommand("hostname");
    }
    else {
        wchar_t computername[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD computername_len = MAX_COMPUTERNAME_LENGTH + 1;
        GetComputerName(computername, &computername_len);
        wcout << computername << endl;
    }
}

bool changeDirectory(const string& path) {
    wstring widePath(path.begin(), path.end());
    return SetCurrentDirectory(widePath.c_str());
}

void removeFile(const string& filename) {
    wstring wideFilename(filename.begin(), filename.end());
    if (!DeleteFile(wideFilename.c_str())) {
        cerr << "Failed to remove file: " << filename << endl;
    }
}

void printCurrentPath() {
    cout << filesystem::current_path() << endl;
}

void printCurrentDate() {
    auto t = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y");
    cout << oss.str() << endl;
}

void printCurrentTime() {
    auto t = std::time(nullptr);
    std::tm tm;
    localtime_s(&tm, &t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    cout << oss.str() << endl;
}

void echoText(const vector<string>& tokens) {
    for (size_t i = 1; i < tokens.size(); i++) {
        cout << tokens[i] << " ";
    }
    cout << endl;
}

string formatFileSize(uintmax_t size) {
    const char* sizes[] = { "B", "KB", "MB", "GB", "TB" };
    int order = 0;
    double dblSize = static_cast<double>(size);

    while (dblSize >= 1024 && order < (sizeof(sizes) / sizeof(sizes[0])) - 1) {
        order++;
        dblSize /= 1024;
    }

    stringstream fileSizeStream;
    fileSizeStream << fixed << setprecision(2) << dblSize << " " << sizes[order];
    return fileSizeStream.str();
}

string getFileIcon(const string& extension) {
    if (filesystem::is_directory(extension)) {
        return "🗁";
    }
    else {
        return "▯";
    }
}

// READFILE FUNCTION HERE
void readFile(const string& filepath) {
    ifstream file(filepath);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filepath << endl;
    }
}


void listFilesInDirectory() {
    for (const auto& entry : filesystem::directory_iterator(filesystem::current_path())) {
        auto filepath = entry.path();
        auto filename = filepath.filename().string();
        auto filesize = entry.file_size();
        // size файла
        string readable_size = formatFileSize(filesize);
		// get file icon
        string icon = getFileIcon(filepath.extension().string());

        cout << icon << " " << filename << " | " << readable_size << endl;
    }
}

void printSystemInfo() {
    // windows version
    RTL_OSVERSIONINFOW rovi = { 0 };
    rovi.dwOSVersionInfoSize = sizeof(rovi);
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        auto RtlGetVersion = (LONG(WINAPI*)(PRTL_OSVERSIONINFOW))GetProcAddress(hMod, "RtlGetVersion");
        if (RtlGetVersion != nullptr) {
            RtlGetVersion(&rovi);
            std::wcout << L"Windows Version: " << rovi.dwMajorVersion << L"." << rovi.dwMinorVersion << L"." << rovi.dwBuildNumber << std::endl;
        }
    }
	// computer name
	wchar_t computername[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD computername_len = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(computername, &computername_len);
	wcout << L"Computer Name: " << computername << endl;

	// username
	wchar_t username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserName(username, &username_len);
	wcout << L"Username: " << username << endl;

	// number of processors
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	wcout << L"Number of Processors: " << si.dwNumberOfProcessors << endl;
}

void clearScreen() {
    system("cls");
}



vector<string> splitString(const string& str) {
    istringstream iss(str);
    vector<string> tokens;
    string token;
    while (iss >> quoted(token)) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    string input;
    while (true) {
		if (isWslMode) {
			cout << "coolshell@wsl" << "> ";
		}
        else {
            cout << "coolshell@main" << "> ";
        }
        getline(cin, input);

        auto tokens = splitString(input);
        if (tokens.empty()) continue;

        string command = tokens[0];
        if (command == "help") {
            cout << "Available commands:\n";
            cout << "  git         Inits a git-shell\n";
			cout << "  cd          Changes path\n";
			cout << "  rm          Removes the file without annoying /s /q\n";
			cout << "  ls          Lists files in current directory\n";
			cout << "  rf          Prints the content of the file\n";
			cout << "  clear       Clears the screen\n";
			cout << "  pwd         Prints current path\n";
			cout << "  date        Prints current date\n";
			cout << "  time        Prints current time\n";
			cout << "  hostname    Prints current computer name\n";
			cout << "  echo        Prints text\n";
            cout << "  download    Downloads a package\n";
			cout << "  wsl         Inits a wsl-shell\n";
			cout << "  purple      Inits a purple-shell\n";
            cout << "  info        Prints info about system\n";
			cout << "  help        Prints this help message\n";
            cout << "  exit        Exits the shell\n";
        } else if (command == "exit") {
            break;
        } else if (command == "git") {
            runGitShell();
        }
		else if (command == "purple") {
            runpurpleShell();
		}
        else if (command == "cd" && tokens.size() > 1) {
            if (!changeDirectory(tokens[1])) {
                cerr << "Failed to change directory to: " << tokens[1] << endl;
            }
        }
		else if (command == "rf" && tokens.size() > 1) {
			readFile(tokens[1]);
		}
        else if (command == "rm" && tokens.size() > 1) {
            removeFile(tokens[1]);
        }
        else if (command == "ls") {
            listFilesInDirectory();
        }
        else if (command == "clear") {
            clearScreen();
        } 
        else if (command == "pwd") {
            printCurrentPath();
        }
        else if (command == "date") {
            printCurrentDate();
        }
        else if (command == "time") {
            printCurrentTime();
        }
        else if (command == "hostname") {
            printComputerName();
        }
        else if (command == "echo") {
            echoText(tokens);
        }
        else if (command == "download" && tokens.size() > 1) {
            downloadPackage(tokens[1]);
        }
        else if (command == "wsl") {
            initWslShell();
        }
		else if (command == "info") {
			printSystemInfo();
		}
        else {
			// Исполнение команды в консоли (либо если в wsl режиме, тогда в wsl)
			runCommand(command);
        }
    }
    return 0;
}
