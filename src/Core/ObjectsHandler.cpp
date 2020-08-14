#include "ObjectsHandler.hpp"
#include "UpdateManager.hpp"
#include "../Object/Object.hpp"

unsigned int ObjectsHandler::DestroyedObjectIndex;
vector<Object*> ObjectsHandler::Objects;

void ObjectsHandler::Load() {

    Objects = vector<Object*>();
}

void ObjectsHandler::ExecuteCode(ExecutionCode code) {

    if (code == UPDATE) {
        UpdateManager::Update();
        return;
    }

    unsigned int Size = Objects.size();

    for (unsigned int Index = 0; Index < Size; Index++) {

        DestroyedObjectIndex = -1;

        switch (code) {
            case LOAD:
                Objects[Index]->ExecuteCode(code);
                break;
            case RENDER:
                Objects[Index]->ExecuteCode(code);
                break;
            case POSRENDER:
                Objects[Index]->ExecuteCode(code);
                break;
        }

        if (DestroyedObjectIndex <= Index)
            Index--;   
    }
}

void ObjectsHandler::AddObject(Object* object) {
    Objects.push_back(object);
}

void ObjectsHandler::Clear() {
    Objects.clear();
    Objects.shrink_to_fit();
}