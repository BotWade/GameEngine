#include "Debug.hpp"

void Debug::Alert(const char* Message) {

    cout << "\033[33m[Log] \033[0m" << Message << endl;
}

void Debug::Alert(string Message) {
    Alert(Message.c_str());
}

void Debug::Error(const char* Message) {

    cout << "\033[31m[Error] \033[0m" << Message << endl;
}

void Debug::Error(string Message) {
    Error(Message.c_str());
}