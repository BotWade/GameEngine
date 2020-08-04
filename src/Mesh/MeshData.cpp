#include "MeshData.hpp"

MeshData::MeshData() {

    MeshName = "";
    Vertices = vector<Vertex>();
    Indices  = vector<unsigned int>();
    MeshMaterial = Material();
}

MeshData::MeshData(vector<Vertex>& vertices, vector<unsigned int>& indices) {

    MeshName = "";
    Vertices = vertices;
    Indices  = indices;
    MeshMaterial = Material();
}
