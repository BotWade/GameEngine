#include "Mesh.hpp"

bool Mesh::LoadMesh(const char* FilePath) {

    Loader loader;
    loader.LoadFile(FilePath);
    
    Path = FilePath;

    Data = loader.LoadedMeshes[0].Vertices;
    Indices = loader.LoadedIndices;
    IndexSize = Indices.size();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Data[0]) * Data.size(), Data.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), Indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return true;
}

bool Mesh::LoadEmptyMesh(size_t DataSize, size_t IndexSize) {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, DataSize, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, NULL, GL_DYNAMIC_DRAW);

    glBindVertexArray(0);


    return true;
}

void Mesh::ProcessAttributes(int Type, unsigned long TypeSize, Shader* shader) {
    
    glBindVertexArray(VAO);

    int Stride = 0;

    for (auto& n : shader->Attributes)
        Stride += n;

    Stride *= TypeSize;
    
    int Offset = 0;

    int Size = shader->Attributes.size();
    for (int Index = 0; Index < Size; Index++) {
        glVertexAttribPointer(Index, shader->Attributes[Index], Type, GL_FALSE, Stride, (void*)(Offset * TypeSize));
        glEnableVertexAttribArray(Index);
        Offset += shader->Attributes[Index];
    }

    AttributesProcessed = true;

    glBindVertexArray(0);
}

void Mesh::UpdateData(void* data, size_t Size) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, Size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::UpdateIndex(void* data, size_t Size, size_t Count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, Size, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    IndexSize = Count;
}

void Mesh::Bind() {
    glBindVertexArray(VAO);
}

void Mesh::UnBind() {
    glBindVertexArray(0);
}

void Mesh::Draw() {
    glDrawElements(GL_TRIANGLES, IndexSize, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VBO);
    delete[] Path;
    Data.clear();
    Data.shrink_to_fit();
    Indices.clear();
    Indices.shrink_to_fit();
}