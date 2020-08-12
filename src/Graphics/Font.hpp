#ifndef _CHARACTER_
#define _CHARACTER_

#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Math/Vector2.hpp"
#include "../Math/Matrix4.hpp"

using namespace std;

struct Character {
    unsigned int TextureID;
    Vector2 Size;
    Vector2 Bearing;
    unsigned int Advance;
};

struct Font {
    string Name;
    FT_Face FontFace;
    map<char, Character> Characters;
    bool FaildToLoadFont;

    void LoadFont(const char* FontPath);
};

struct FontMeshData {
    unsigned int VAO;
    unsigned int VBO;
};

#endif