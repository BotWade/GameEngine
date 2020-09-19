#include "TextRenderer.hpp"
#include "Texture.hpp"
#include "Uniform.hpp"
#include "Renderer.hpp"
#include "Vulkan/VulkanDescriptor.hpp"
#include "Vulkan/PipelineManager.hpp"
#include "Vulkan/VulkanPipeLine.hpp"
#include "Vulkan/RenderPassManager.hpp"
#include "Vulkan/VulkanDeviceManager.hpp"
#include "Vulkan/CommandBufferManager.hpp"
#include "../Math/Vector4.hpp"
#include "../Mesh/Mesh.hpp"
#include "../Core/ShaderManager.hpp"
#include "../Core/TextureManager.hpp"
#include "../Core/Window.hpp"

stb_fontchar TextRenderer::stbFontData[STB_NUM_CHARS];
Mesh* TextRenderer::mesh = new Mesh();
Texture* TextRenderer::texture = new Texture();;
VulkanDescriptor* TextRenderer::uniformData = new VulkanDescriptor();
VulkanPipeLine* TextRenderer::pipeLine = new VulkanPipeLine();
Vector4* TextRenderer::VertexData;
int TextRenderer::NumLetters;
VulkanCommandBuffer* TextRenderer::CommandBuffer;

vector<VkVertexInputBindingDescription>* TextVertex::GetBindingDescription() {
    vector<VkVertexInputBindingDescription>* bindingDescription = new vector<VkVertexInputBindingDescription>(1);
    (*bindingDescription)[0].binding = 0;
    (*bindingDescription)[0].stride = sizeof(TextVertex);
    (*bindingDescription)[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

vector<VkVertexInputAttributeDescription>* TextVertex::GetAttributeDescriptions() {
    vector<VkVertexInputAttributeDescription>* attributeDescriptions = new vector<VkVertexInputAttributeDescription>(2);
    (*attributeDescriptions)[0].binding = 0;
    (*attributeDescriptions)[0].location = 0;
    (*attributeDescriptions)[0].format = VK_FORMAT_R32G32_SFLOAT;
    (*attributeDescriptions)[0].offset = offsetof(TextVertex, Position);
    (*attributeDescriptions)[1].binding = 0;
    (*attributeDescriptions)[1].location = 1;
    (*attributeDescriptions)[1].format = VK_FORMAT_R32G32_SFLOAT;
    (*attributeDescriptions)[1].offset = offsetof(TextVertex, UV);

    return attributeDescriptions;
}

void TextRenderer::Init() {
    unsigned char font24pixels[STB_FONT_HEIGHT][STB_FONT_WIDTH];
	STB_FONT_NAME(stbFontData, font24pixels, STB_FONT_HEIGHT);
    
    mesh->AlocateData(TEXTOVERLAY_MAX_CHAR_COUNT * sizeof(Vector4));

    texture->LoadTexture(&font24pixels[0][0], Vector2(STB_FONT_WIDTH, STB_FONT_HEIGHT), 1);

    Shader* shader = ShaderManager::GetShader("Data/Shaders/Letter.shader");
    uniformData->PrepareUniforms(shader);
    uniformData->GetUniform("fontTexture")->SetTexture(texture);
    uniformData->Generate(shader);

    pipeLine->PreparePipelineLayout();
    pipeLine->vkPipeLineData.InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    pipeLine->SetVertexInputInfo(TextVertex::GetBindingDescription(), TextVertex::GetAttributeDescriptions());
    pipeLine->CreatePipelineLayout(uniformData);
    pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
    pipeLine->Name = "TextRender";

    PipelineManager::AddPipeline(pipeLine);
    CommandBuffer = CommandBufferManager::GetOrCreate("TextRenderer");
}

void TextRenderer::BeginTextUpdate() {
    vkMapMemory(VulkanDeviceManager::GetSelectedDevice(), mesh->vertexBufferMemory, 0, VK_WHOLE_SIZE, 0, (void**)&VertexData);
    NumLetters = 0;
}

void TextRenderer::AddText(string Text, Vector2 Position, float TextSize, TextAlign Align) {
    uint32_t FirstChar = STB_FONT_consolas_24_latin1_FIRST_CHAR;

    if (VertexData == nullptr) {
        Debug::Alert("Failed To Write Text Vertex Data Is NULL");
        return;
    }

    Vector2 Size = Window::GetSize();

    Vector2 CharSize = Vector2(TextSize / Size.X, TextSize / Size.Y);
    Position.X = Position.X / Size.X * 2.0f - 1.0f;
    Position.Y = Position.Y / Size.Y * 2.0f - 1.0f;

    float Width = 0;

    for (char letter : Text)
        Width += stbFontData[(uint32_t)letter - FirstChar].advance * CharSize.X;

    switch (Align) {
        case AlignRight:
            Position.X -= Width;
            break;
        case AlignCenter:
            Position.X -= Width / 2.0f;
            break;
    }

    for (char Letter : Text) {
		stb_fontchar *charData = &stbFontData[(uint32_t)Letter - FirstChar];

		VertexData->X = (Position.X + (float)charData->x0 * CharSize.X);
		VertexData->Y = (Position.Y + (float)charData->y0 * CharSize.Y);
		VertexData->Z = charData->s0;
		VertexData->W = charData->t0;
		VertexData++;

		VertexData->X = (Position.X + (float)charData->x1 * CharSize.X);
		VertexData->Y = (Position.Y + (float)charData->y0 * CharSize.Y);
		VertexData->Z = charData->s1;
		VertexData->W = charData->t0;
		VertexData++;

		VertexData->X = (Position.X + (float)charData->x0 * CharSize.X);
		VertexData->Y = (Position.Y + (float)charData->y1 * CharSize.Y);
		VertexData->Z = charData->s0;
		VertexData->W = charData->t1;
		VertexData++;

		VertexData->X = (Position.X + (float)charData->x1 * CharSize.X);
		VertexData->Y = (Position.Y + (float)charData->y1 * CharSize.Y);
		VertexData->Z = charData->s1;
		VertexData->W = charData->t1;
		VertexData++;

		Position.X += charData->advance * CharSize.X;

		NumLetters++;
    }
}

void TextRenderer::EndTextUpdate() {
    vkUnmapMemory(VulkanDeviceManager::GetSelectedDevice(), mesh->vertexBufferMemory);
    VertexData = nullptr;
    UpdateCommandBuffers();
}

void TextRenderer::UpdateCommandBuffers() {
    Vulkan::commandBuffer = CommandBuffer;
    
    ///Temporary Fix
    vkDeviceWaitIdle(VulkanDeviceManager::GetSelectedDevice());

    for (size_t Index = 0; Index < CommandBuffer->commandBuffers.size(); Index++) {
        Vulkan::CurrentCommandBuffer = Index;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
            Debug::Error("failed to begin recording command buffer!");

        Renderer::Prepare();

        pipeLine->Bind();
        uniformData->Bind(pipeLine);
        
        VkDeviceSize offsets = 0;
        vkCmdBindVertexBuffers(Vulkan::GetCurrentCommandBuffer(), 0, 1, &mesh->vertexBuffer, &offsets);

        for (uint32_t SubIndex = 0; SubIndex < NumLetters; SubIndex++) {
            vkCmdDraw(Vulkan::GetCurrentCommandBuffer(), 4, 1, SubIndex * 4, 0);
        }

        vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

        if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
            Debug::Error("failed to record command buffer!");
    }

    CommandBufferManager::AddToQueuePool(CommandBuffer);
}

void TextRenderer::Clean() {
    uniformData->Clean();
    mesh->Clean();
    texture->Clear();
}