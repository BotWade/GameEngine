#include "UpdateManager.hpp"
#include <iostream> 
using namespace std; 
using namespace std::chrono; 

vector<UpdateGroup*> UpdateManager::Groups;

void UpdateManager::CreateGroup(string Name){
    UpdateGroup* group = new UpdateGroup();
    group->Name = Name;
    Groups.push_back(group);
    group->Init();
}

void UpdateManager::AddToGroup(string Name, Object* object){
    
    size_t Size = Groups.size();

    for (size_t Index = 0; Index < Size; Index++)
        if (Groups[Index]->Name == Name)
            Groups[Index]->Objects.push_back(object);
}

void UpdateManager::Update() {

    size_t Size = Groups.size(); 

    for (size_t Index = 0; Index < Size; Index++) {
        Groups[Index]->Update = true;
    }

    bool Working = true;

    while (Working) {
        Size = Groups.size();
        Working = false;

        for (size_t Index = 0; Index < Size; Index++) {
            if (Groups[Index]->Update)
                Working = true;
        }
    }
}