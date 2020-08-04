#include "Mesh.hpp"

bool Mesh::LoadMesh(const char* FilePath) {

    Loader loader;
    loader.LoadFile(FilePath);
    
    Path = FilePath;

    Data = loader.LoadedMeshes[0].Vertices;
    Indices = loader.LoadedIndices;
    
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

void Mesh::Bind() {
    glBindVertexArray(VAO);
}

void Mesh::UnBind() {
    glBindVertexArray(0);
}

void Mesh::Draw() {
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
}