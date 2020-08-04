#include "Object.hpp"
#include "Component.hpp"

void Object::AddComponent(Component* component) {
    component->object = this;
    Components.push_back(component);
}

void Object::ExecuteCode(ExecutionCode code) {
    size_t Size = Components.size();
    
    for (size_t Index = 0; Index < Size; Index++) {
        switch (code) {
            case LOAD:
                Components[Index]->Load();
                break;
            case UPDATE:
                Components[Index]->Update();
                break;
            case RENDER:
                Components[Index]->Render();
                break;
            case POSRENDER:
                Components[Index]->PosRender();
                break;
        }
    }
    
}