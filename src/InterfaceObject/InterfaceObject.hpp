#ifndef _INTERFACEOBJECT_
#define _INTERFACEOBJECT_

#include "../Math/Vector4.hpp"
#include "../Graphics/Shader.hpp"
#include "../Mesh/Mesh.hpp"

#include <vector>

using namespace std;

enum ConstraintMode {
    PIXEL = 0,      //Value Is Set In Pixels
    RELATIVE = 1,   //Value Is Set In Percentage
    ASPECT = 2      //Value Is Set In Units
};

class InterfaceObject {
public:
    bool Active;
    
    Shader* shader;
    Mesh* mesh;

    InterfaceObject* Parent;

    vector<InterfaceObject*> Children;

    ConstraintMode xConstraint;
    ConstraintMode yConstraint;
    ConstraintMode wConstraint;
    ConstraintMode hConstraint;

    Vector4 BackgroundColor;
    Vector4 ForegroundColor;
    Vector2 Position;
    Vector2 Size;
    Vector4 RenderPosition;

    InterfaceObject();

    void LoadObject(const char* Shader);

    virtual void Load();
    virtual void Update();
    virtual void Render();
};

#endif