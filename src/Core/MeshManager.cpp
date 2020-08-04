#include "MeshManager.hpp"

vector<Mesh*> MeshManager::meshes;

void MeshManager::Init() {

    meshes = vector<Mesh*>();
}

Mesh* MeshManager::GetMesh(const char* Location) {

    size_t Size = meshes.size();
    
    for (int Index = 0; Index < Size; Index++) {
        if (meshes[Index]->Path == Location)
            return meshes[Index];
    }

    Mesh* mesh = new Mesh();
    mesh->LoadMesh(Location);
    meshes.push_back(mesh);
    return mesh;
}