#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>
#include <openssl/types.h>

void initWslShell(bool& isWslMode);
bool runCommand(const std::string& cmd, bool& isWslMode);
bool checkPurpleExeExists();
void runGitShell(bool& isWslMode);
void runpurpleShell(bool& isWslMode);
void scanShell();
bool isComment(const std::string& line);
std::string removeSingleLineComment(const std::string& line);
std::string removeMultiLineComment(const std::string& line, bool& inComment);
std::string removeCommentsFromFile(const std::string& inputFile, const std::string& outputFile);
void downloadPackage(const std::string& packageName, bool& isWslMode);
void printComputerName(bool& isWslMode);
bool changeDirectory(const std::string& path);
void removeFile(const std::string& filename);
void printCurrentPath();
void printCurrentDate();
void printCurrentTime();
void echoText(const std::vector<std::string>& tokens);
std::string formatFileSize(uintmax_t size);
std::string getFileIcon(const std::string& extension);
void readFile(const std::string& filepath);
void listFilesInDirectory();
void printSystemInfo();
void clearScreen();
std::string calcHash(const std::string& file_path, const EVP_MD* md_type);
std::vector<std::string> splitString(const std::string& str);

#endif // FUNCTIONS_H
