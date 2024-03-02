#include "functions.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <filesystem>
#include <fstream>
#include <thread>
#include <lmcons.h>
#include <sstream>
#include <regex>
#include <openssl/evp.h>
#include <iomanip>

using namespace std;

void initWslShell(bool &isWslMode) {
    isWslMode = !isWslMode; // Переключает состояние WSL-режима
    cout << "WSL Shell " << (isWslMode ? "activated." : "deactivated.") << endl;
}

bool runCommand(const string& cmd, bool& isWslMode) {
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

void runGitShell(bool& isWslMode) {
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

        runCommand("git " + gitCmd, isWslMode);
    }
}

void runpurpleShell(bool& isWslMode) {
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

            bool result = runCommand("purple --command" + purplecmd, isWslMode); // !!
            if (!result) {
                runCommand("C:\\Users\\%USERNAME%\\Downloads\\purple.exe --command " + purplecmd, isWslMode);
            }
        }
    }
}

struct HostInfo {
    string ip_address;
    string hostname;
    string status;
    string mac_address; // Добавлено для хранения MAC-адреса
};

void scanShell() {
    string scanCmd;
    cout << "Entering scan-shell utility. Type 'exit' to leave." << endl;
    while (true) {
        cout << "coolshell@scan" << "> ";
        getline(cin, scanCmd);

        bool isVerbose = false;

        if (scanCmd == "exit") {
            break;
        }
        else if (scanCmd == "help") {
            cout << "Commands:" << endl;
            cout << "exit - Exit scan-shell mode" << endl;
            cout << "help - Show this help message" << endl;
            cout << "arp  - ARP packets utility (not working)" << endl;
            cout << "show - Showing all available IP addresses" << endl;
            cout << "verbose - Enabling/Disabling verbose" << endl;
        }
        else if (scanCmd == "arp") {
            // Код для работы с ARP пакетами
        }
        else if (scanCmd == "verbose") {
            isVerbose = !isVerbose;
            cout << "Verbose mode changed. " << isVerbose << endl;
        }
        else if (scanCmd == "show") {
            string network;

            cout << "Enter network (e.g. 192.168.1.0/24): ";
            cin >> network;

            string command = "nmap -sn " + network;

            cout << "Scanning network, please wait..." << endl;

            system((command + " > scan_results.txt").c_str());

            ifstream scanResults("scan_results.txt");

            if (scanResults.is_open()) {
                vector<HostInfo> hosts;

                string line;
                while (getline(scanResults, line)) {
                    if (line.find("Nmap scan report") != string::npos) {
                        HostInfo host;
                        host.ip_address = line.substr(line.find("for ") + 4);
                        host.hostname = "";
                        host.status = "";
                        host.mac_address = "";
                        hosts.push_back(host);
                    }
                    else if (line.find("Host is") != string::npos) {
                        hosts.back().status = line.substr(line.find("is ") + 3);
                    }
                    else if (line.find("MAC Address") != string::npos) {
                        size_t pos = line.find("MAC Address: ");
                        if (pos != string::npos) {
                            hosts.back().mac_address = line.substr(pos + 13);
                        }
                    }
                    else if (line.find("hostname") != string::npos) {
                        hosts.back().hostname = line.substr(line.find("hostname") + 9);
                    }
                }

                cout << "IP Address\t\tHostname\tMAC Address\t\tStatus" << endl;

                for (const auto& host : hosts) {
                    cout << host.ip_address << "\t\t" << host.hostname << "\t\t" << host.mac_address << "\t\t" << host.status << endl;
                }

                scanResults.close();
            }
            else {
                cerr << "Unable to open scan results file." << endl;
            }
        }
        else {
            cout << "Unknown command. Type 'help' for a list of commands." << endl;
        }
    }
}

// START OF DELETING COMMENTS
void removeComments(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream ifs(inputFile);
    if (!ifs.is_open()) {
        std::cerr << "Error opening input file: " << inputFile << '\n';
        return;
    }

    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    // Regular expression pattern for C++-style comments and string literals
    std::regex pattern(R"((/\*[^*]*\*+([^/*][^*]*\*+)*\/)|//.*$)");

    // Replace comments with empty strings
    content = std::regex_replace(content, pattern, "");

    std::ofstream ofs(outputFile);
    if (!ofs.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << '\n';
        return;
    }

    ofs << content;
    ofs.close();
}

// END OF DELETING COMMENTS


void downloadPackage(const string& packageName, bool &isWslMode) {
    if (isWslMode) {
        cout << "Using apt-get to download " << packageName << "..." << endl;
        runCommand("sudo apt-get install " + packageName, isWslMode);
    }
    else {
        cout << "Using winget to download " << packageName << "..." << endl;
        runCommand("winget install " + packageName, isWslMode);
    }
}

void printComputerName(bool& isWslMode) {
    if (isWslMode) {
        runCommand("hostname", isWslMode);
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
    cout << filesystem::current_path() << '\n';
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

void readFile(const string& filepath) {
    ifstream file(filepath);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
    else {
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

std::string calcHash(const std::string& file_path, const EVP_MD* md_type) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    EVP_DigestInit_ex(mdctx, md_type, NULL);

    std::ifstream file(file_path, std::ifstream::binary);
    if (!file.is_open()) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Could not open file " + file_path);
    }

    const int bufSize = 32768;
    char* buffer = new char[bufSize];
    while (file.good()) {
        file.read(buffer, bufSize);
        EVP_DigestUpdate(mdctx, buffer, file.gcount());
    }
    file.close();
    delete[] buffer;

    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < md_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)md_value[i];
    }

    return ss.str();
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