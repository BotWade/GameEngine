#include "ShipCollider.hpp"
#include "../../Core/MeshManager.hpp"
#include "../../Core/Input.hpp"
#include "../../Graphics/Renderer.hpp"

void ShipCollider::Load() {

    mesh = MeshManager::GetMesh("Data/Meshes/Sphere.obj");

    if (object->Parent == nullptr)
        Debug::Alert("ShipCollider Doesn't Have A Parent");

    object->transform.localScale = Vector3(100, 100, 100);

    collider = new Collider();
    object->AddComponent(collider);
    collider->LoadCollider(mesh);
    CollisionManager::AddCollider(collider);
}

void ShipCollider::Render() {
    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
        ///Draw Collider Triangles
        Matrix4 Model = object->transform.ModelMatrix();
        size_t Size = collider->Triangles.size();
        for (size_t Index = 0; Index < Size; Index++)
            Renderer::DrawTriangle(collider->Triangles[Index] * Model, Vector3(0, 1, 0));
    }
}