#ifndef _FILEMANAGER_
#define _FILEMANAGER_

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <linux/limits.h>
#include <libgen.h>

#include "Debug.hpp"

using namespace std;

class FileManager {
private:
    static string GamePath;

public:
    static void UpdateGamePath();
    static string GetGamePath();

    static bool FileExists(const char* FileLocation);
    
    static bool GetFile(const char* FileLocation, ifstream& File);
    static bool GetFileHex(const char* FileLocation, ifstream& File);
};

#endif