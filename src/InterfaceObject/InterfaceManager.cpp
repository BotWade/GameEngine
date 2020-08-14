#include "InterfaceManager.hpp"
#include "../Core/Window.hpp"

vector<InterfaceObject*> InterfaceManager::Objects;
InterfaceObject* InterfaceManager::Canvas;
bool InterfaceManager::Interacted;

void InterfaceManager::Init() {
    Canvas = new InterfaceObject();
    Canvas->Size = Window::GetSize();
    Canvas->Position = Vector2(0, 0);
    Objects = vector<InterfaceObject*>();
    Interacted = false;
}

void InterfaceManager::AddObject(InterfaceObject* Object) {

    Object->Parent = Canvas;
    Objects.push_back(Object);
}

void InterfaceManager::ExecuteCode(ExecutionCode code) {

    size_t Size = Objects.size();

    for (size_t Index = 0; Index < Size; Index++) {
        switch (code) {
        case LOAD:
            Objects[Index]->Load();
            break;
        case UPDATE:
            Objects[Index]->Update();
            break;
        case RENDER:
            Objects[Index]->Render();
            break;
        }
    }
}

void InterfaceManager::Clear() {

    Objects.clear();
    Objects.shrink_to_fit();
}