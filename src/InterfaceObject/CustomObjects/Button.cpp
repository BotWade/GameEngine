#include "Button.hpp"
#include "../../Graphics/Renderer.hpp"
#include "../../Core/Input.hpp"
#include "../InterfaceManager.hpp"

void Button::Update() {
    
    InterfaceObject::Update();

    if (Input::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
        if (OnClick != nullptr && IsPointInsideRect(Input::MousePositionYInv, RenderPosition)) {
            InterfaceManager::Interacted = true;
            OnClick();
        }
    }
}

void Button::Render() {
    InterfaceObject::Render();
    Renderer::SetScissor(RenderPosition);
    Renderer::DrawText("Arial", Text, RenderPosition.Xy(), 1, ForegroundColor.Xyz());
    Renderer::SetScissorDefault();
}