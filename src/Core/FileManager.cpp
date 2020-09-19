#include "FileManager.hpp"

string FileManager::GamePath = "";

void FileManager::UpdateGamePath() {
    
    char result[PATH_MAX];
    size_t len = readlink("/proc/self/exe", result, PATH_MAX);
    
    if (len != -1)
        GamePath = (string)dirname(result) + "/";

    Debug::Alert("Game Path - " + GamePath);
}

string FileManager::GetGamePath() {
    return GamePath;
}

bool FileManager::FileExists(const char* FileLocation) {
    struct stat buffer;   
    return (stat (FileLocation, &buffer) == 0); 
}

bool FileManager::GetFile(const char* FileLocation, ifstream& File) {

    string fileLocation = GamePath + FileLocation;

    if (!FileExists(fileLocation.c_str()))
        return false;

    File.open(fileLocation.c_str());

    if (!File)
        return false;

    return true;
}

bool FileManager::GetFileBinary(string& Path, vector<char>& Data) {
    ifstream file(Path, ios::ate | ios::binary);

    if (!file.is_open())
        return false;

    size_t fileSize = (size_t) file.tellg();
    vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    Data = buffer;

    return true;
}

bool FileManager::GetFileHex(const char* FileLocation, ifstream& File) {

    if (!FileExists(FileLocation))
        return false;

    File.open(FileLocation, ios::binary);

    if (!File)
        return false;

    File >> std::noskipws;

    return true;
}
