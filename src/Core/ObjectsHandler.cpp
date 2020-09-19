#include "ObjectsHandler.hpp"
#include "UpdateManager.hpp"
#include "../Object/Object.hpp"

unsigned int ObjectsHandler::DestroyedObjectIndex;
vector<Object*> ObjectsHandler::Objects;
nanoseconds ObjectsHandler::UpdateTime;
nanoseconds ObjectsHandler::RenderTime;
nanoseconds ObjectsHandler::PosRenderTime;
bool ObjectsHandler::AlreadyLoaded = false;

void ObjectsHandler::Load() {

    Objects = vector<Object*>();
}

void ObjectsHandler::ExecuteCode(ExecutionCode code) {

    if (code == LOAD)
        AlreadyLoaded = true;

    if (code == UPDATE) {
        auto Pre = high_resolution_clock::now();
        UpdateManager::Update();
        auto Now = high_resolution_clock::now();
        UpdateTime = duration_cast<nanoseconds>(Now - Pre);
        return;
    }

    unsigned int Size = Objects.size();

    auto Pre = high_resolution_clock::now();
    for (unsigned int Index = 0; Index < Size; Index++) {

        DestroyedObjectIndex = -1;
        Objects[Index]->ExecuteCode(code);
        if (DestroyedObjectIndex <= Index)
            Index--;   
    }

    auto Now = high_resolution_clock::now();

    switch (code) {
        case RENDER:
            RenderTime = duration_cast<nanoseconds>(Now - Pre);
            break;
        case POSRENDER:
            PosRenderTime = duration_cast<nanoseconds>(Now - Pre);
            break;
    }
}

void ObjectsHandler::AddObject(Object* object) {
    Objects.push_back(object);

    if (AlreadyLoaded) {
        object->ExecuteCode(LOAD);
        object->ExecuteCode(PRERENDER);
    }
}

void ObjectsHandler::Clear() {
    Objects.clear();
    Objects.shrink_to_fit();
}