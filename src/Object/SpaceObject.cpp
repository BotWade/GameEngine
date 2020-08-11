#include "SpaceObject.hpp"
#include "../Graphics/Renderer.hpp"
#include "../Core/Camera.hpp"
#include "../../Core/ShaderManager.hpp"
#include "../../Core/MeshManager.hpp"
#include "../../Collisions/CollisionManager.hpp"

SpaceObject::SpaceObject() {
    isOrbitable = true;
}

void SpaceObject::LoadObject(const char* Shader, const char* Model) {

    shader = ShaderManager::GetShader(Shader);
    mesh = MeshManager::GetMesh(Model);
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);
}

void SpaceObject::LoadCollider(Mesh* colliderMesh) {

    collider = new Collider();
    object->AddComponent(collider);
    collider->LoadCollider(colliderMesh);
    CollisionManager::AddCollider(collider);
}

void SpaceObject::Update() {

    model = Camera::ProjectionView * object->transform.ModelMatrix();
}

void SpaceObject::Render() {

    shader->Bind();
    
    glUniformMatrix4fv(shader->GetUniformId("MVP"), 1, GL_FALSE, &model.col0.X);
    
    mesh->Bind();
    mesh->Draw();
    mesh->UnBind();
    shader->UnBind();
}