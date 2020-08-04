#ifndef _OBJECT_
#define _OBJECT_

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    Object();

    vector<Component*> Components;
    vector<Object*> Children;

    void AddComponent(Component* component);
    void AddChild(Object* child);
    void ExecuteCode(ExecutionCode code);
};

#endif