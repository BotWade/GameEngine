#ifndef _INTERFACEOBJECT_
#define _INTERFACEOBJECT_

#include "../Math/Matrix4.hpp"
#include "../Core/ObjectsHandler.hpp"

#include <vector>
#include <string>

using namespace std;

enum ConstraintMode {
    PIXEL = 0,      //Value Is Set In Pixels
    RELATIVE = 1,   //Value Is Set In Percentage
    ASPECT = 2      //Value Is Set In Units
};

struct PointerFunctionData {
    void (*F)(void *);
    void* Param;
};

struct InterfaceUniformData {
    Matrix4 Projection;
    Vector4 BackgroundColor;
};


class Shader;
class Mesh;
class VulkanDescriptor;
class VulkanPipeLine;
class VulkanCommandBuffer;

class InterfaceObject {
public:
    bool Active;
    bool Visibel;
    string Tag;

    Shader* shader;
    Mesh* mesh;
    VulkanDescriptor* uniformData;
    VulkanPipeLine* pipeLine;
    VulkanCommandBuffer* commandBuffer;
    InterfaceUniformData data;

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

    PointerFunctionData OnClick;
    PointerFunctionData OnHover;

    InterfaceObject();
    
    /*
    * Load Shader (Maybe More In The Future)
    * @param Shader The Location Of The Shader 
    */
    void LoadObject(const char* Shader);
    /*
    * Add Interface Object As Children
    * @param object The Child You Want To Add This Object 
    */
    void AddChild(InterfaceObject* object);
    
    /*
    * Update RenderPosition Data 
    */
    void UpdatePosition();
    /*
    * Update Mesh Data
    */
    void UpdateMesh();

    virtual void Load();
    virtual void Update();
    virtual void PreRender();
    virtual void Render();
    virtual void PosRender();
    virtual void Clean();

    /*
    * Execute Children
    * @param code The Code You Want The Children To Execute
    */
    void ExecuteChildren(ExecutionCode code);
};

#endif