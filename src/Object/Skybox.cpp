#include "Skybox.hpp"
#include "../Core/MeshManager.hpp"
#include "../Core/ShaderManager.hpp"
#include "../Core/TextureManager.hpp"

void Skybox::LoadSkybox(const char* Skybox, const char* Mesh, const char* Shader) {

    texture = TextureManager::GetTexture(Skybox, CUBEMAP);
    shader = ShaderManager::GetShader(Shader);

    mesh = MeshManager::GetMesh(Mesh);
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);
}

void Skybox::Update() {

}

void Skybox::PosRender() {
    
    glDepthFunc(GL_LEQUAL);
    texture->Bind();
    shader->Bind();
    
    Matrix4 View = Camera::Projection * Matrix4(Matrix3(Camera::View));
    glUniformMatrix4fv(shader->GetUniformId("VP"), 1, GL_FALSE, &View.col0.X);

    mesh->Bind();
    mesh->Draw();
    mesh->UnBind();

    texture->UnBind();
    shader->UnBind();
    glDepthFunc(GL_LESS);
}