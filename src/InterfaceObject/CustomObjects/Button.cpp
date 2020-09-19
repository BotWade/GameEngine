#include "Button.hpp"
#include "../../Graphics/TextRenderer.hpp"
#include "../../Core/Input.hpp"
#include "../InterfaceManager.hpp"

void Button::Render() {
    InterfaceObject::Render();
    //Renderer::SetScissor(RenderPosition);
    //Renderer::DrawText("Arial", Text, RenderPosition.Xy(), 1, ForegroundColor.Xyz());
    TextRenderer::AddText(Text, RenderPosition.Xy(), 2);
    //Renderer::SetScissorDefault();
}