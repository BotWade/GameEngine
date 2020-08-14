#ifndef _RENDERER_
#define _RENDERER_

#include <vector>

#include "Font.hpp"
#include "Shader.hpp"
#include "../Core/Window.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Triangle.hpp"

class Renderer {
public:
    static FT_Library freeTypeLibrary;
    static bool faildToLoadFreetype;
    static Font* defaultFont;
    static FontMeshData fontMesh;
    static Shader* defaultFontShader;

    static vector<Font*> fonts;

    static void Init();
    static void Clear();
    static void RendererClear();
    static void Swap();
    static void UpdateMatrix();
    static void DrawRay(Ray ray, float Distance = 10.0f, Vector3 Color = Vector3(1, 0, 0));
    static void DrawLine(Vector3 Origin, Vector3 End, Vector3 Color = Vector3(1, 0, 0));
    static void DrawTriangle(Triangle triangle, Vector3 Color = Vector3(1, 0, 0));
    static void DrawText(string FontName, string Text, Vector2 Position, float Scale, Vector3 Color, Shader* shader = defaultFontShader);
    static int DrawLetter(Font* font,char Letter, Vector2 Position, float Scale);
};

#endif