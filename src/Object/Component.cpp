#include "Component.hpp"

void Component::SetObject(Object* newObject) {
    object = newObject;
    transform = &newObject->transform;
}

void Component::Load() { }
void Component::Update() { }
void Component::OnCollision(RayTestResult result) { }
void Component::PreRender() { }
void Component::Render() { }
void Component::PosRender() { }
void Component::Clean() { }