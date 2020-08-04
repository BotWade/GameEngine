#ifndef _ASTEROIDS_
#define _ASTEROIDS_


#include "../Component.hpp"
#include "../../Graphics/Shader.hpp"
#include "../../Core/Debug.hpp"
#include "../../Core/Camera.hpp"
#include "../../Mesh/Mesh.hpp"
#include "../../Graphics/Texture.hpp"
#include "../../Collisions/Collider.hpp"

class Asteroid : public Component {
public:

    Mesh* mesh;
    Shader* shader;

    Matrix4 Model;

    Collider* collider;
    int colliderDataIndex;
    
    void Load() override;
    void Update() override;
    void Render() override;
};

#endif