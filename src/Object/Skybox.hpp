#ifndef _SKYBOX_
#define _SKYBOX_

#define GL_GLEXT_PROTOTYPES
#include <GLFW/glfw3.h>

#include "Component.hpp"
#include "../Core/Camera.hpp"
#include "../Graphics/Shader.hpp"
#include "../Graphics/Texture.hpp"
#include "../Mesh/Mesh.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Matrix3.hpp"

class Skybox : public Component {
public:
    Texture texture;
    Shader* shader;
    Mesh* mesh;

    void LoadSkybox(const char* Skybox, const char* Mesh, const char* Shader);

    void Update() override;
    void PosRender() override;
};

#endif