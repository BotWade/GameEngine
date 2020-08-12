#include "PlayerShip.hpp"
#include "../../Core/Camera.hpp"
#include "../../Core/MeshManager.hpp"
#include "../../Core/UpdateManager.hpp"
#include "../../Graphics/Renderer.hpp"

void PlayerShip::Load() {
    
    LoadObject("Data/Shaders/Basic.shader", "Data/Meshes/Main Ship.obj");
    LoadCollider(mesh);

    movementMesh = MeshManager::GetMesh("/Data/Meshes/Sphere.obj");

    object->transform.localPosition = Vector3(0, 0, 0);
    Camera::FirstPersonMode = false;
    Camera::Center = object->transform.localPosition;
    Camera::Position = Vector3(0, 0, 10);
    Camera::Zoom = 10;
}

void PlayerShip::Update() {
    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT))
        CollisionManager::ProcessRay(Ray(Camera::Position, Screen2World(Input::MousePosition), true), this);

    if (isOrbiting) {

        Vector3 StartPosition = object->transform.localPosition;
        Vector3 OrbitPoint = MovementTarget + orbitDistance * ((StartPosition- MovementTarget)/(Distance(StartPosition, MovementTarget)));

        float distance = Distance(OrbitPoint, StartPosition);
        float DeltaSpeed = TimeHelper::GetDeltaTime() * acceleration;

        object->transform.localPosition = Vector3::MoveTowards(StartPosition, (distance > 1 ? OrbitPoint : Vector3::RotateAround(StartPosition, MovementTarget, Vector3(0, 1, 0), DeltaSpeed)), DeltaSpeed);
        
        Camera::OriginalPos = StartPosition + Vector3(0, 0, Camera::Zoom);
        Camera::Center = StartPosition;

        Quaternion lookRotation = Quaternion::LookRotation(StartPosition, (distance > 1 ? MovementTarget : object->transform.localPosition));
        
        //If Source And Target Are Equal lookRotation Becomes Broken (nan, nan, nan, nan) this way we can test if it is broken
        if (lookRotation.Quat.X == lookRotation.Quat.X)
            object->transform.localRotation = Quaternion::Slerp(object->transform.localRotation, lookRotation, TimeHelper::GetDeltaTime());
    }

    Ship::Update();
}

void PlayerShip::OnCollision(RayTestResult result) {
    
    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)) {
        SpaceObject* obj = result.result[0]->Hitted->object->GetComponent<SpaceObject>();
        if (obj != nullptr && obj->isOrbitable) {
            MovementTarget = result.result[0]->Target;
            isOrbiting = true;
            inOrbit = false;
            orbitDistance = 10.0f;
            ///Acceleration = Velocity^2/Radius there for pre set speed multiplyed by it self to avoid unnecessary calculations
            speed = 20.0f;
            acceleration = speed / orbitDistance;
        }
    }
}

    
void PlayerShip::PosRender() {

    for (size_t Index = 0; Index < UpdateManager::Groups.size(); Index++)
        Renderer::DrawText("Arial", "Update Group " + to_string(Index) + " Time - " + to_string(UpdateManager::Groups[Index]->UpdateTime.count()), Vector2(0, 20 * Index), 0.4, Vector3(1, 1, 1));
}