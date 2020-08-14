#include "Object.hpp"
#include "Component.hpp"

Object::Object() {
    transform.object = this;
    Parent = nullptr;
}

void Object::AddComponent(Component* component) {
    component->object = this;
    Components.push_back(component);
}

void Object::AddChild(Object* child) {
    child->Parent = this;
    Children.push_back(child);
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
    
    Size = Children.size();

    for (size_t Index = 0; Index < Size; Index++)
        Children[Index]->ExecuteCode(code);
}

Object::~Object() {
    transform.object = nullptr;
    Parent = nullptr;
    Components.clear();
    Components.shrink_to_fit();
    Children.clear();
    Children.shrink_to_fit();
}