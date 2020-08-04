#ifndef _LOADER_
#define _LOADER_

#include "../../Core/FileManager.hpp"
#include "../../Math/String.hpp"
#include "../MeshData.hpp"
#include "Algorithm.hpp"

class Loader {
public:

	vector<MeshData> LoadedMeshes;	
	vector<Vertex> LoadedVertices;
	vector<unsigned int> LoadedIndices;
	vector<Material> LoadedMaterials;

    ~Loader();

    bool LoadFile(string Path);

    void GenVerticesFromRawOBJ(vector<Vertex> &oVerts, const vector<Vector3>& iPositions, const vector<Vector2>& iTCoords, const vector<Vector3>& iNormals, string icurline);
    void VertexTriangulation(vector<unsigned int>& oIndices, const vector<Vertex>& iVerts);
    bool LoadMaterials(string Path);
};

#endif