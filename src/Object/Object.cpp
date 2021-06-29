#include "Object.hpp"
#include "Component.hpp"

Object::Object() {
    transform.object = this;
    Persistant = true;
    Parent = nullptr;
    Components = { };
}

void Object::AddComponent(Component* component) {
    component->SetObject(this);
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
            case PRERENDER:
                Components[Index]->PreRender();
                break;
            case RENDER:
                Components[Index]->Render();
                break;
            case POSRENDER:
                Components[Index]->PosRender();
                break;
            case CLEAN:
                Components[Index]->Clean();
                break;
        }
    }
    
    Size = Children.size();

    for (size_t Index = 0; Index < Size; Index++)
        Children[Index]->ExecuteCode(code);
}