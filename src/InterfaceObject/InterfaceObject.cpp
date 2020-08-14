#include "InterfaceObject.hpp"
#include "../Core/ShaderManager.hpp"
#include "../Graphics/Renderer.hpp"
#include <algorithm>

InterfaceObject::InterfaceObject() {

    xConstraint = yConstraint = wConstraint = hConstraint = PIXEL;
    yConstraint = RELATIVE;
    hConstraint = RELATIVE;
    BackgroundColor = Vector4(1, 1, 1, 1);
    ForegroundColor = Vector4(0, 0, 0, 1);
    Position = Vector2(100, 10);
    Size = Vector2(100, 80);

    Children = vector<InterfaceObject*>();
}

void InterfaceObject::Load() {
    LoadObject("Data/Shaders/Interface.shader");
    mesh = new Mesh();
    mesh->LoadEmptyMesh(sizeof(float) * 4 * 2, sizeof(int) * 6);

    int indices[] = {0,2,1,
                     0,3,2};

    mesh->UpdateIndex(indices, sizeof(int) * 6, 6);
    mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);
}

void InterfaceObject::LoadObject(const char* Shader) {

    shader = ShaderManager::GetShader(Shader);
}

void InterfaceObject::Update() { 

    if (xConstraint == PIXEL && yConstraint == PIXEL && wConstraint == PIXEL && hConstraint == PIXEL) {
        RenderPosition = Vector4(Position, Size);
        return;
    }

    ///Process Size
    if (wConstraint == RELATIVE)
        RenderPosition.Z = Parent->Size.X / 100 * Size.X;
    if (hConstraint == RELATIVE)
        RenderPosition.W = Parent->Size.Y / 100 * Size.Y;

    if (wConstraint == PIXEL)
        RenderPosition.Z = Size.X;
    if (hConstraint == PIXEL)
        RenderPosition.W = Size.Y;

    if (wConstraint == ASPECT && hConstraint == ASPECT)
        RenderPosition.Z = RenderPosition.W = 10;
    else if (wConstraint == ASPECT)
        RenderPosition.Z = RenderPosition.W * Size.X;
    else if (hConstraint == ASPECT)
        RenderPosition.W = RenderPosition.Z * Size.Y;

    ///Process Position
    if (xConstraint == RELATIVE)
        RenderPosition.X = Parent->Size.X / 100 * Position.X;
    if (yConstraint == RELATIVE)
        RenderPosition.Y = Parent->Size.Y / 100 * Position.Y;

    if (xConstraint == PIXEL)
        RenderPosition.X = Position.X;
    if (yConstraint == PIXEL)
        RenderPosition.Y = Position.Y;

    if (xConstraint == ASPECT && yConstraint == ASPECT)
        RenderPosition.X = RenderPosition.Y = 10;
    else if (xConstraint == ASPECT)
        RenderPosition.X = RenderPosition.X * Position.X;
    else if (yConstraint == ASPECT)
        RenderPosition.Y = RenderPosition.Y * Position.Y;

}

void InterfaceObject::Render() { 

    shader->Bind();

    glUniform4fv(shader->GetUniformId("backgroundColor"), 1, &BackgroundColor.X);
    glUniformMatrix4fv(shader->GetUniformId("projection"), 1, GL_FALSE, &Window::OrthoProjection.col0.X);

    float vertices[4][2] = {{RenderPosition.X, RenderPosition.Y},
                            {RenderPosition.X, RenderPosition.Y + RenderPosition.W},
                            {RenderPosition.X + RenderPosition.Z, RenderPosition.Y + RenderPosition.W},
                            {RenderPosition.X + RenderPosition.Z, RenderPosition.Y} };

    mesh->Bind();
    mesh->UpdateData(vertices, sizeof(vertices));
    mesh->Draw();
    mesh->UnBind();
    shader->UnBind();
}