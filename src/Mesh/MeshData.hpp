#ifndef _MESHDATA_
#define _MESHDATA_

#include <string>
#include <vector>

#include "../Graphics/Vertex.hpp"
#include "../Graphics/Material.hpp"

using namespace std;

struct MeshData {

    string MeshName;

    vector<Vertex> Vertices;
    vector<unsigned int> Indices;

    Material MeshMaterial;

    MeshData();
    MeshData(vector<Vertex>& vertices, vector<unsigned int>& indices);

};


#endif