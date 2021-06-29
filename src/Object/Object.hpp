#ifndef _OBJECT_
#define _OBJECT_

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "../Core/TimeHelper.hpp"
#include "../Core/ObjectsHandler.hpp"
#include "../Math/Transform.hpp"

using namespace std;

class Component;

class Object {
public:
    string Tag;
    Transform transform;
    Object* Parent;
    
    /**
    * If persistant Scene::Load won't delete
    */
    bool Persistant;
    
    Object();

    vector<Component*> Components;
    vector<Object*> Children;

    /**
    * Add Component To Object
    * @param component The Component To Add
    */
    void AddComponent(Component* component);
    /**
    * Add Children To Object
    * @param child The Child You Want To Add
    */
    void AddChild(Object* child);
    /**
    * Execute Code In Components And Children
    * @param code The Code You Want To Executes 
    */
    void ExecuteCode(ExecutionCode code);
    /**
    * @tparam T The Type Of The Component You Want
    * @return Returns A Pointer Of The Type T If It Doesnt Exist It Returns nullptr;
    */
    template<class T> T* GetComponent() {
        
        size_t Size = Components.size();

        for (size_t Index = 0; Index < Size; Index++){
            T* comp = dynamic_cast<T*>(Components[Index]);
            if (comp != nullptr)
                return comp;
        }
        
        return nullptr;        
    }
};

#endif