#ifndef _MESHMANAGER_
#define _MESHMANAGER_

#include "../Mesh/Mesh.hpp"
#include <vector>

using namespace std;

class MeshManager {
public:
    static vector<Mesh*> meshes;

    static void Init();
    static Mesh* GetMesh(const char* Location);
    static void Clear();
};

#endif