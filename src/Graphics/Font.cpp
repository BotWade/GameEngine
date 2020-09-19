#include "Font.hpp"
#include "Renderer.hpp"
#include "../Core/FileManager.hpp"
#include "Vulkan/Vulkan.hpp"

void Font::LoadFont(const char* Path) {

    if (!FileManager::FileExists((FileManager::GetGamePath() + (string)Path).c_str())) {
        Debug::Alert("Font doesn't exist - " + (string)Path);
        return;
    }

    if (!Renderer::faildToLoadFreetype && FT_New_Face(Renderer::freeTypeLibrary, (FileManager::GetGamePath() + (string)Path).c_str(), 0, &FontFace)) {
        FaildToLoadFont = true;
        Debug::Alert("Faild To Load Font!");
        return;
    }

    FT_Set_Pixel_Sizes(FontFace, 0, 48);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(FontFace, c, FT_LOAD_RENDER)) {
            Debug::Alert("FreeType Failed To Load Glyph " + (string)((char*)c));
            continue;
        }

        Character character;
        character.Size = Vector2(FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows);
        character.texture = new Texture();
        //character.texture->LoadTexture(FontFace->glyph->bitmap.buffer, character.Size, 1);
        character.Advance = FontFace->glyph->advance.x;
        character.Bearing = Vector2(FontFace->glyph->bitmap_left, FontFace->glyph->bitmap_top);
        character.quad = new Mesh();
        character.quad->LoadPrimitive(QUAD);
        character.commandBuffer = CommandBufferManager::GetOrCreate((string)((char*)c));

        Vulkan::commandBuffer = character.commandBuffer;
        for (size_t Index = 0; Index < character.commandBuffer->commandBuffers.size(); Index++) {
            Vulkan::CurrentCommandBuffer = Index;

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
                Debug::Error("failed to begin recording command buffer!");
            
            Renderer::Prepare();

            vkCmdBindPipeline(Vulkan::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine->graphicsPipeline);
            
            character.quad->Bind();
            //shader->BindDescriptors(pipeLine);
            character.quad->Draw();

            vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

            if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
                Debug::Error("failed to record command buffer!");
        }
        
        Characters.insert(pair<char, Character>(c, character));
    }        
}

void Font::DrawLetter(char C) {
    CommandBufferManager::AddToQueuePool(Characters[C].commandBuffer);
}

void Font::Clean() {
    for (std::map<char, Character>::iterator c = Characters.begin(); c != Characters.end(); ++c) {
        c->second.texture->Clear();
        c->second.quad->Clean();
    }
}