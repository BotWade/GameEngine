#include "Ship.hpp"
#include "../../Math/Matrix3.hpp"
#include "../../Core/ObjectsHandler.hpp"
#include "../../Core/ShaderManager.hpp"
#include "../../Core/MeshManager.hpp"
#include "../../Core/UpdateManager.hpp"
#include "../RayDrawer.hpp"

void Ship::Load() {
    
    shader = ShaderManager::GetShader("Data/Shaders/Basic.shader");
    Debug::Alert("Finished Generating Shaders");

    mesh = MeshManager::GetMesh("Data/Meshes/Main Ship.obj");
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);
    movementMesh = MeshManager::GetMesh("/Data/Meshes/Sphere.obj");
    
    Debug::Alert("Finished Loading Mesh");

    object->transform.localPosition = Vector3(0, 0, 0);

    Camera::FirstPersonMode = false;
    Camera::Center = object->transform.localPosition;
    Camera::Position = Vector3(0, 0, 10);

    collider = new Collider();
    object->AddComponent(collider);
    collider->LoadCollider(mesh);
    CollisionManager::AddCollider(collider);
}


void Ship::Update() {

    ///Camera Rotation

    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        CollisionManager::ProcessRay(Ray(Camera::Position, Screen2World(Input::MousePosition)), this);
    }

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
        Object * ray = new Object();
        RayDrawer* rayDrawer = new RayDrawer(Ray(Camera::Position, Screen2World(Input::MousePosition)));
        ray->AddComponent(rayDrawer);
        ObjectsHandler::AddObject(ray);
    }

    model = Camera::ProjectionView * object->transform.ModelMatrix();

}

void Ship::OnCollision(RayTestResult result) {
    Debug::Alert("Mouse Hit - " + result.result[0]->Hitted->object->Tag);
}

void Ship::Render() {

    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        ///Draw Collider Triangles
        Matrix4 Model = object->transform.ModelMatrix();
        size_t Size = collider->Triangles.size();
        for (size_t Index = 0; Index < Size; Index++)
            Renderer::DrawTriangle(collider->Triangles[Index] * Model);
    }
    else {
        shader->Bind();

        glUniformMatrix4fv(shader->GetUniformId("MVP"), 1, GL_FALSE, &model.col0.X);
        
        mesh->Bind();
        mesh->Draw();
        mesh->UnBind();
        shader->UnBind();
    }
}

void Ship::PosRender() {

    for (size_t Index = 0; Index < UpdateManager::Groups.size(); Index++)
        Renderer::DrawText("Arial", "Update Group " + to_string(Index) + " Time - " + to_string(UpdateManager::Groups[Index]->UpdateTime.count()), Vector2(0, 20 * Index), 0.4, Vector3(1, 1, 1));
}