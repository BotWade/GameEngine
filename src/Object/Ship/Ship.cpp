#include "Ship.hpp"
#include "../../Core/Camera.hpp"

void Ship::Load() {
    
    LoadObject("Data/Shaders/Basic.shader", "Data/Meshes/Default.obj");
    LoadCollider(mesh);
}


void Ship::Update() {

    ///Camera Rotation
    model = Camera::ProjectionView * object->transform.ModelMatrix();
}

void Ship::Render() {

    shader->Bind();
    
    glUniformMatrix4fv(shader->GetUniformId("MVP"), 1, GL_FALSE, &model.col0.X);
    
    mesh->Bind();
    mesh->Draw();
    mesh->UnBind();
    shader->UnBind();
}