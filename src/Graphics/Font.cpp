#include "Font.hpp"
#include "Renderer.hpp"
#include "../Core/FileManager.hpp"

void Font::LoadFont(const char* Path) {

    if (!FileManager::FileExists((FileManager::GetGamePath() + (string)Path).c_str())) {
        Debug::Alert("Font doesn't exist - " + (string)Path);
        return;
    }

    if (!Renderer::faildToLoadFreetype && FT_New_Face(Renderer::freeTypeLibrary, (FileManager::GetGamePath() + (string)Path).c_str(), 0, &FontFace)) {
        FaildToLoadFont = true;
        Debug::Alert("Faild To Load Font!");
    }

    FT_Set_Pixel_Sizes(FontFace, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(FontFace, c, FT_LOAD_RENDER)) {
            Debug::Alert("Faild To Load Font Face - " + c);
            continue;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, FontFace->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character =  {
            Texture,
            Vector2(FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows),
            Vector2(FontFace->glyph->bitmap_left, FontFace->glyph->bitmap_top),
            FontFace->glyph->advance.x
        };

        Characters.insert(pair<char, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}