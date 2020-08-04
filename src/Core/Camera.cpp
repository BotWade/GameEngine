#include "Camera.hpp"
#include "TimeHelper.hpp"
#include "../Math/Matrix3.hpp"
#include "../Graphics/Renderer.hpp"

Vector3 Camera::Position;
Vector3 Camera::Center;
Vector3 Camera::Up;
Quaternion Camera::Rotation;

Matrix4 Camera::Projection(1);
Matrix4 Camera::View(1);
Matrix4 Camera::ProjectionView(1);

bool Camera::FirstPersonMode = true;

float yaw = 0.0f;
float pitch = 0.0f;

Vector3 Euler;
Vector3 OriginalPos = Vector3(0, 0, 10);
Quaternion quat;


void Camera::Update() {

    Vector3 Front;
    Front.X = cos(Radians(yaw)) * cos(Radians(pitch));
    Front.Y = sin(Radians(pitch));
    Front.Z = sin(Radians(yaw)) * cos(Radians(pitch));
        
    View = Matrix4::LookAtLH(Position, FirstPersonMode ? Position + Vector3(0, 0, 1) : Center, Vector3(0, 1, 0));
    
    Vector2 WindowSize = Window::GetSize();
    Projection = Matrix4::PerspectiveFovLH(Radians(45.0f), WindowSize.X, WindowSize.Y, 0.1f, 1000.0f);

    ProjectionView = Projection * View;

    float y = 0.0f;
    float x = 0.0f;
    float Speed = 0.5f;
    float WheelSpeed = 0.5f;

    if (Input::ScrollOffset != Vector2(0, 0))
        OriginalPos.Z -= Input::ScrollOffset.Y * WheelSpeed;

    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {

        x = -1 * Input::MouseOffset.X * Speed * TimeHelper::GetDeltaTime();
        y = Input::MouseOffset.Y * Speed * TimeHelper::GetDeltaTime();
        
        Euler.X += x;

        if (Euler.Y + y > -M_PI_2 && Euler.Y + y < M_PI_2)
            Euler.Y += y;

        quat = Quaternion::EulerToQuaternion(Vector3(Euler.Y, Euler.X, 0));

    }

    Position = quat * OriginalPos;

    Renderer::UpdateMatrix();
}