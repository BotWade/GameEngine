#include "MeshManager.hpp"

vector<Mesh*> MeshManager::meshes;

void MeshManager::Init() {

    meshes = vector<Mesh*>();
}

Mesh* MeshManager::GetMesh(const char* Location) {

    size_t Size = meshes.size();
    
    for (size_t Index = 0; Index < Size; Index++) {
        if (meshes[Index]->Path == Location)
            return meshes[Index];
    }

    Mesh* mesh = new Mesh();
    mesh->LoadMesh(Location);
    meshes.push_back(mesh);
    return mesh;
}

void MeshManager::Clear() {

    for (Mesh* mesh : meshes)
        mesh->Clean();

    meshes.clear();
    meshes.shrink_to_fit();
}