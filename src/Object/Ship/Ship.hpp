#ifndef _Ship_
#define _Ship_

#include "../Component.hpp"
#include "../../Collisions/Collider.hpp"
#include "../../Graphics/Shader.hpp"
#include "../../Core/Debug.hpp"
#include "../../Core/Camera.hpp"
#include "../../Mesh/Mesh.hpp"
#include "../../Graphics/Texture.hpp"

class Ship : public Component {
public:

    Mesh* mesh;
    Mesh* movementMesh;
    Shader* shader;

    Matrix4 model;
    
    Collider* collider;
    
    void Load() override;
    void Update() override;
    void OnCollision(RayTestResult result) override;
    void Render() override;
    void PosRender() override;
};

#endif