#include "Transform.hpp"
#include "../Object/Object.hpp"

Transform::Transform() {
    localScale = Vector3(1, 1, 1);
    localRotation = Quaternion();
    localPosition = Vector3(0, 0, 0);
}

Vector3 Transform::Up() {
    return Rotation() * Vector3::Up();
}

Quaternion Transform::Rotation() {
    return (object->Parent == nullptr ? localRotation : localRotation * object->Parent->transform.Rotation()); 
}

Vector3 Transform::Scale() {
    return (object->Parent == nullptr ? localScale : localScale * object->Parent->transform.Scale());
}

Vector3 Transform::Position() {
    return (object->Parent == nullptr ? localPosition : localPosition + object->Parent->transform.Position());
}

Matrix4 Transform::ModelMatrix() {
    return Matrix4::Translate(Position()) * Matrix4::FromQuaternion(Rotation()) * Matrix4::Scale(Scale());
}