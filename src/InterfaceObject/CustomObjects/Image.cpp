#include "Image.hpp"
#include "../../Core/Window.hpp"

void Image::Load() {

    LoadObject("Data/Shaders/InterfaceImage.shader");
    mesh = new Mesh();
    mesh->LoadEmptyMesh(sizeof(float) * 4 * 4, sizeof(int) * 6);

    int indices[] = {0,2,1,
                     0,3,2};

    mesh->UpdateIndex(indices, sizeof(int) * 6, 6);
    mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);
}

void Image::Render() {

    if (Active) {
        shader->Bind();
        texture->Bind();

        glUniform4fv(shader->GetUniformId("backgroundColor"), 1, &BackgroundColor.X);
        glUniformMatrix4fv(shader->GetUniformId("projection"), 1, GL_FALSE, &Window::OrthoProjection.col0.X);

        float vertices[4][4] = {{RenderPosition.X, RenderPosition.Y                                         , 0.0f, 0.0f},
                                {RenderPosition.X, RenderPosition.Y + RenderPosition.W                      , 0.0f, 1.0f},
                                {RenderPosition.X + RenderPosition.Z, RenderPosition.Y + RenderPosition.W   , 1.0f, 1.0f},
                                {RenderPosition.X + RenderPosition.Z, RenderPosition.Y                      , 1.0f, 0.0f} };

        mesh->Bind();
        mesh->UpdateData(vertices, sizeof(vertices));
        mesh->Draw();
        mesh->UnBind();
        texture->UnBind();
        shader->UnBind();
    }
}