#include "Scene.hpp"
#include "../Core/UpdateManager.hpp"
#include "../Core/ObjectsHandler.hpp"
#include "./Object.hpp"

void Scene::Load() {
    //NOTE: Not sure if this can cause a memory leak, more research needed
    for (int Index = 0; Index < ObjectsHandler::Objects.size(); Index++) {
        if (!ObjectsHandler::Objects[Index]->Persistant) {
            ObjectsHandler::Objects.erase(ObjectsHandler::Objects.begin() + Index);
        }
    }

    for (int Index = 0; Index < UpdateManager::Groups.size(); Index++) {
        for (int SubIndex = 0; SubIndex < UpdateManager::Groups[Index]->Objects.size(); SubIndex++) {
            if (!UpdateManager::Groups[Index]->Objects[SubIndex]->Persistant || UpdateManager::Groups[Index]->Objects[SubIndex] == nullptr) {
                UpdateManager::Groups[Index]->Objects.erase(UpdateManager::Groups[Index]->Objects.begin() + SubIndex);
            }
        }
    }
}