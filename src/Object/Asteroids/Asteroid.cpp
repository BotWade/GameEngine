#include "Asteroid.hpp"
#include "../../Graphics/Renderer.hpp"
#include "../../Core/ShaderManager.hpp"
#include "../../Core/MeshManager.hpp"
#include "../../Collisions/CollisionManager.hpp"

void Asteroid::Load() {
    
    shader = ShaderManager::GetShader("Data/Shaders/Basic.shader");
    
    //Debug::Alert("Finished Generating Shaders");

    mesh = MeshManager::GetMesh("Data/Meshes/Asteroids.obj");
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);
        
    //Debug::Alert("Finished Loading Mesh");

    object->transform.localPosition = Vector3(rand() % (200 - (-200) + 1) + -200, rand() % (200 - (-200) + 1) + -200, rand() % (200 - (-200) + 1) + -200);

    collider = new Collider();
    object->AddComponent(collider);
    collider->LoadCollider(mesh);
    CollisionManager::AddCollider(collider);
}

void Asteroid::Update() {

    Model = Camera::ProjectionView * object->transform.ModelMatrix();

}

void Asteroid::Render() {

    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        ///Draw Collider Triangles
        size_t Size = collider->Triangles.size();
        for (size_t Index = 0; Index < Size; Index++)
            Renderer::DrawTriangle(collider->Triangles[Index] * object->transform.ModelMatrix());
    }
    else {
        shader->Bind();

        glUniformMatrix4fv(shader->GetUniformId("MVP"), 1, GL_FALSE, &Model.col0.X);
        
        mesh->Bind();
        mesh->Draw();
        mesh->UnBind();
        shader->UnBind();
    }
}