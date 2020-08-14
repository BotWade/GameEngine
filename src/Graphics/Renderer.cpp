#include "Renderer.hpp"
#include "../Core/Debug.hpp"
#include "../Core/ShaderManager.hpp"
#include "../Core/Camera.hpp"

FT_Library Renderer::freeTypeLibrary;
bool Renderer::faildToLoadFreetype;
Font* Renderer::defaultFont;
FontMeshData Renderer::fontMesh;
Shader* Renderer::defaultFontShader;

vector<Font*> Renderer::fonts;

void Renderer::Init() {
    faildToLoadFreetype = false;

    if (FT_Init_FreeType(&freeTypeLibrary)) {
        faildToLoadFreetype = true;
        Debug::Alert("Faild To Load FreeType2");
    }

    if (!faildToLoadFreetype) {
        defaultFontShader = ShaderManager::GetShader("Data/Shaders/Letter.shader");
        defaultFontShader->Bind();
        defaultFont = new Font();
        defaultFont->LoadFont("Data/Fonts/Arial.ttf");

        glGenVertexArrays(1, &fontMesh.VAO);
        glGenBuffers(1, &fontMesh.VBO);
        glBindVertexArray(fontMesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, fontMesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        defaultFontShader->UnBind();
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::Clear() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::UpdateMatrix() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(&Camera::Projection.col0.X);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(&Camera::View.col0.X);
}


void Renderer::Swap() {
    
    glfwSwapBuffers(Window::window);
    glfwPollEvents();
}


void Renderer::DrawRay(Ray ray, float Distance, Vector3 Color) {
    glBegin(GL_LINE_STRIP);
        glColor3f(Color.X, Color.Y, Color.Z);
        glVertex3f(ray.Origin.X, ray.Origin.Y, ray.Origin.Z);
        Vector3 End = ray.GetPoint(Distance);
        glVertex3f(End.X, End.Y, End.Z);
    glEnd();
}

void Renderer::DrawLine(Vector3 Origin, Vector3 End, Vector3 Color) {
    glBegin(GL_LINE_STRIP);
        glColor3f(Color.X, Color.Y, Color.Z);
        glVertex3f(Origin.X, Origin.Y, Origin.Z);
        glVertex3f(End.X, End.Y, End.Z);
    glEnd();}

void Renderer::DrawTriangle(Triangle triangle, Vector3 Color) {
    glBegin(GL_TRIANGLES);
        glColor3f(Color.X, Color.Y, Color.Z);
        glVertex3f(triangle.Pos1.X, triangle.Pos1.Y, triangle.Pos1.Z);
        glVertex3f(triangle.Pos2.X, triangle.Pos2.Y, triangle.Pos2.Z);
        glVertex3f(triangle.Pos3.X, triangle.Pos3.Y, triangle.Pos3.Z);
    glEnd();
}

void Renderer::DrawText(string FontName, string Text, Vector2 Position, float Scale, Vector3 Color, Shader* shader) {
    
    if (faildToLoadFreetype)
        return;
    
    shader->Bind();
    glUniform3f(shader->GetUniformId("textColor"), Color.X, Color.Y, Color.Z);
    glUniformMatrix4fv(shader->GetUniformId("projection"), 1, GL_FALSE, &Window::OrthoProjection.col0.X);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(fontMesh.VAO);

    Font* font = nullptr;

    for (size_t Index = 0; Index < fonts.size(); Index++)
        if (fonts[Index]->Name == FontName)
            font = fonts[Index];
    
    if (font == nullptr)
        font = defaultFont;

    if (font == nullptr || font->FaildToLoadFont)
        return;

    for (string::const_iterator c = Text.begin(); c != Text.end(); c++)
        Position.X += (DrawLetter(font, *c, Position, Scale) >> 6) * Scale;

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shader->UnBind();
}

int Renderer::DrawLetter(Font* font, char Letter, Vector2 Position, float Scale) {
    Character ch = font->Characters[Letter];

    Vector2 Pos = Vector2(Position.X + ch.Bearing.X * Scale, Position.Y - (ch.Size.Y - ch.Bearing.Y) * Scale);
    Vector2 Size = Vector2(ch.Size.X * Scale, ch.Size.Y * Scale);

    float Vertices[6][4] = {
        {Pos.X          , Pos.Y + Size.Y, 0.0f, 0.0f},
        {Pos.X          , Pos.Y         , 0.0f, 1.0f},
        {Pos.X + Size.X , Pos.Y         , 1.0f, 1.0f},

        {Pos.X          , Pos.Y + Size.Y, 0.0f, 0.0f},
        {Pos.X + Size.X , Pos.Y         , 1.0f, 1.0f},
        {Pos.X + Size.X , Pos.Y + Size.Y, 1.0f, 0.0f}
    };

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, fontMesh.VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return ch.Advance;
}

void Renderer::RendererClear() {

    size_t Size = fonts.size();
    for (size_t Index = 0; Index < Size; Index++)
        FT_Done_Face(fonts[Index]->FontFace);
    FT_Done_Face(defaultFont->FontFace);
    FT_Done_FreeType(freeTypeLibrary);

    fonts.clear();
    fonts.shrink_to_fit();
    delete defaultFontShader;
    delete defaultFont;
    
    glDeleteBuffers(1, &fontMesh.VBO);
    glDeleteVertexArrays(1, &fontMesh.VAO);
}