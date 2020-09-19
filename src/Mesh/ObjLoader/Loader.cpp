#include "Loader.hpp"
#include "math.h"

Loader::~Loader() {
    LoadedMeshes.clear();
}

bool Loader::LoadFile(string Path) {

    Path = FileManager::GetGamePath() + Path;
    ifstream file(Path);

    if (!file.is_open())
        return false;
    
    LoadedMeshes.clear();
    LoadedVertices.clear();
    LoadedIndices.clear();

    vector<Vector3> Positions;
    vector<Vector2> TexCoords;
    vector<Vector3> Normals;

    vector<Vertex> Vertices;
    vector<unsigned int> Indices;

    vector<string> MeshMatNames;

    bool Listening = false;
    string MeshName;

    MeshData TempMesh;

    string curline;
    while (getline(file, curline)) {

        string firstToken = FirstToken(curline);

        if (firstToken == "o" || firstToken == "g" || curline[0] == 'g')  {
            if (!Listening) {
                
                Listening = true;

                if (firstToken == "o" || firstToken == "g")
                    MeshName = Tail(curline);
                else
                    MeshName = "unnamed";
            }
            else {
                
                if (!Indices.empty() && !Vertices.empty()) {
                    
                    TempMesh = MeshData(Vertices, Indices);
                    TempMesh.MeshName = MeshName;

                    LoadedMeshes.push_back(TempMesh);

                    Vertices.clear();
                    Indices.clear();
                    MeshName.clear();

                    MeshName = Tail(curline);
                }
                else {

                    if (firstToken == "o" || firstToken == "g")
                        MeshName = Tail(curline);
                    else
                        MeshName = "unnamed";   
                }
            }                    
        }
        else if (firstToken == "v") {

            vector<string> spos;
            Vector3 vpos;
            Split(Tail(curline), spos, " ");

            vpos.X = stof(spos[0]);
            vpos.Y = stof(spos[1]);
            vpos.Z = stof(spos[2]);

            Positions.push_back(vpos);
        }
        else if (firstToken == "vt") {
            
            vector<string> stex;
            Vector2 vtex;
            Split(Tail(curline), stex, " ");

            vtex.X = stof(stex[0]);
            vtex.Y = stof(stex[1]);

            TexCoords.push_back(vtex);
        }
        else if (firstToken == "vn") {
            
            vector<string> snor;
            Vector3 vnor;
            Split(Tail(curline), snor, " ");

            vnor.X = stof(snor[0]);
            vnor.Y = stof(snor[1]);
            vnor.Z = stof(snor[2]);

            Normals.push_back(vnor);
            
        }
        else if (firstToken == "f") {

            vector<Vertex> vVerts;
            GenVerticesFromRawOBJ(vVerts, Positions, TexCoords, Normals, curline);

            for (int Index = 0; Index < (int)vVerts.size(); Index++) {

                Vertices.push_back(vVerts[Index]);
                LoadedVertices.push_back(vVerts[Index]);
            }

            vector<unsigned int> iIndices;
            
            VertexTriangulation(iIndices, vVerts);

            for (int Index = 0; Index < (int)iIndices.size(); Index++) {
                
                unsigned int indnum = (unsigned int)(Vertices.size() - vVerts.size()) + iIndices[Index];
                Indices.push_back(indnum);

                indnum = (unsigned int)(LoadedVertices.size() - vVerts.size()) + iIndices[Index];
                LoadedIndices.push_back(indnum);
            }
        }
        else if (firstToken == "usemtl") {

            MeshMatNames.push_back(Tail(curline));

            if (!Indices.empty() && !Vertices.empty()) {

                TempMesh = MeshData(Vertices, Indices);
                TempMesh.MeshName = MeshName;
                int i = 2;
                while (1) {
                    TempMesh.MeshName = MeshName + "_" + to_string(i);

                    for (auto &m : LoadedMeshes)
                        if (m.MeshName == TempMesh.MeshName)
                            continue;

                    break;
                }

                LoadedMeshes.push_back(TempMesh);

                Vertices.clear();
                Indices.clear();
            }
        }
        else if (firstToken == "mtllib") {

            vector<string> temp;
            Split(Path, temp, "/");

            string pathtomat = "";

            if (temp.size() != 1)
                for (int Index = 0; Index < (int)temp.size() - 1; Index++)
                    pathtomat += temp[Index] + "/";
            
            pathtomat += Tail(curline);

            LoadMaterials(pathtomat);
        }
    }

    if (!Indices.empty() && !Vertices.empty()) {

        TempMesh = MeshData(Vertices, Indices);
        TempMesh.MeshName = MeshName;

        LoadedMeshes.push_back(TempMesh);
    }

    file.close();

    for (int Index = 0; Index < (int)MeshMatNames.size(); Index++) {
        
        string matName = MeshMatNames[Index];

        for (int j = 0; j < (int)LoadedMaterials.size(); j++) {

            if (LoadedMaterials[j].Name == matName) {
                LoadedMeshes[Index].MeshMaterial = LoadedMaterials[j];
                break;
            }
        }
    }

    if (LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty())
        return false;
    else
        return true;
}

void Loader::GenVerticesFromRawOBJ(vector<Vertex> &oVerts, const vector<Vector3>& iPositions, const vector<Vector2>& iTCoords, const vector<Vector3>& iNormals, string icurline) {

    vector<string> sface, svert;
    Vertex vVert;
    Split(Tail(icurline), sface, " ");

    bool noNormal = false;

    for (int Index = 0; Index < (int)sface.size(); Index++) {
        
        int vtype;
        Split(sface[Index], svert, "/");

        if (svert.size() == 1)
            vtype = 1;
        else if (svert.size() == 2)
            vtype = 2;
        else if (svert.size() == 3) {
            if (svert[1] != " ")
                vtype = 4;
            else
                vtype = 3;
        }

        switch (vtype) {
            case 1:
                vVert.Position = getElement(iPositions, svert[0]);
                vVert.TextureCoordinate = Vector2(0, 0);
                noNormal = true;
                oVerts.push_back(vVert);
                break;
            case 2:
                vVert.Position = getElement(iPositions, svert[0]);
                vVert.TextureCoordinate = getElement(iTCoords, svert[1]);
                noNormal = true;
				oVerts.push_back(vVert);
                break;
			case 3:
				vVert.Position = getElement(iPositions, svert[0]);
				vVert.TextureCoordinate = Vector2(0, 0);
				vVert.Normal = getElement(iNormals, svert[2]);
				oVerts.push_back(vVert);
				break;
            case 4:
				vVert.Position = getElement(iPositions, svert[0]);
				vVert.TextureCoordinate = getElement(iTCoords, svert[1]);
				vVert.Normal = getElement(iNormals, svert[2]);
				oVerts.push_back(vVert);
                break;
            default:
                break;
        }
    }

    if (noNormal) {
        Vector3 normal = Cross(oVerts[0].Position - oVerts[1].Position, oVerts[2].Position - oVerts[1].Position);

        for (int Index = 0; Index < (int)oVerts.size(); Index++)
            oVerts[Index].Normal = normal;
    }
}

void Loader::VertexTriangulation(vector<unsigned int>& oIndices, const vector<Vertex>& iVerts) {

    if (iVerts.size() < 3)
        return;

    if (iVerts.size() == 3) {
        oIndices.push_back(0);
        oIndices.push_back(1);
        oIndices.push_back(2);
        return;
    }

    std::vector<Vertex> tVerts = iVerts;

    while (true) {
        for (int Index = 0; Index < (int)tVerts.size(); Index++) {
            Vertex pPrev;
            if (Index == 0)
                pPrev = tVerts[tVerts.size() - 1];
            else
                pPrev = tVerts[Index - 1];

            Vertex pCur = tVerts[Index];

            Vertex pNext;

            if (Index == (int)tVerts.size() - 1)
                pNext = tVerts[0];
            else
                pNext = tVerts[Index + 1];

            if (tVerts.size() == 3) {

                for (int j = 0; j < (int)tVerts.size(); j++) {
                    if (iVerts[j].Position == pCur.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                }

                tVerts.clear();
                break;
            }
            else if (tVerts.size() == 4) {

                for (int j = 0; j < (int)tVerts.size(); j++) {
                    if (iVerts[j].Position == pCur.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                }
                
                Vector3 tempVec;
                for (int j = 0; j < (int)tVerts.size(); j++) {
                    if (tVerts[j].Position != pCur.Position && tVerts[j].Position != pPrev.Position && tVerts[j].Position != pNext.Position) {
                        tempVec = tVerts[j].Position;
                        break;
                    }
                }

                for (int j = 0; j < (int)iVerts.size(); j++) {
                    if (iVerts[j].Position == pPrev.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == pNext.Position)
                        oIndices.push_back(j);
                    if (iVerts[j].Position == tempVec)
                        oIndices.push_back(j);
                }
                tVerts.clear();
                break;
            }

            float angle = AngleBetween(pPrev.Position - pCur.Position, pNext.Position - pCur.Position) * (180 / M_PI);
            
            if (angle <= 0 && angle >= 180)
                continue;

            bool inTri = false;
            
            for (int j = 0; j < (int)iVerts.size(); j++) {

                if (InTriangle(iVerts[j].Position, pPrev.Position, pCur.Position, pNext.Position) && iVerts[j].Position != pPrev.Position && iVerts[j].Position != pCur.Position && iVerts[j].Position != pNext.Position) {
                    inTri = true;
                    break;                    
                }
            }

            if (inTri)
                continue;

            for (int j = 0; j < (int)iVerts.size(); j++) {
                if (iVerts[j].Position == pCur.Position)
                    oIndices.push_back(j);
                if (iVerts[j].Position == pPrev.Position)
                    oIndices.push_back(j);
                if (iVerts[j].Position == pNext.Position)
                    oIndices.push_back(j);
            }

			for (int j = 0; j < int(tVerts.size()); j++) {
				if (tVerts[j].Position == pCur.Position) {
					tVerts.erase(tVerts.begin() + j);
					break;
				}
			}

            Index = -1;
        }

        if (oIndices.size() == 0)
            break;
        if (tVerts.size() == 0)
            break;
    }
}

bool Loader::LoadMaterials(string Path) {

    ifstream file(Path);

    if (!file.is_open())
        return false;

    Material tempMaterial;

    bool listening = false;

    string curline;

    while (getline(file, curline)) {
        
        string firstToken = FirstToken(curline);
        
        if (firstToken == "newmtl") {
            if (!listening) {
                listening = true;
                if (curline.size() > 7)
                    tempMaterial.Name = Tail(curline);
                else
                    tempMaterial.Name = "none";
            }
            else
            {
                LoadedMaterials.push_back(tempMaterial);
                tempMaterial = Material();

                if (curline.size() > 7)
                    tempMaterial.Name = Tail(curline);
                else
                    tempMaterial.Name = "none";
            }
        }
        else if (firstToken == "Ka") {
            vector<string> temp;
            Split(Tail(curline), temp, " ");

            if (temp.size() != 3)
                continue;

            tempMaterial.AmbientColor.X = stof(temp[0]);
            tempMaterial.AmbientColor.Y = stof(temp[1]);
            tempMaterial.AmbientColor.Z = stof(temp[2]);
        }
        else if (firstToken == "Kd") {
            vector<string> temp;
            Split(Tail(curline), temp, " ");

            if (temp.size() != 3)
                continue;

            tempMaterial.DiffuseColor.X = stof(temp[0]);
            tempMaterial.DiffuseColor.Y = stof(temp[1]);
            tempMaterial.DiffuseColor.Z = stof(temp[2]);
        }
        else if (firstToken == "Ks") {
            vector<string> temp;
            Split(Tail(curline), temp, " ");

            if (temp.size() != 3)
                continue;

            tempMaterial.SpecularColor.X = stof(temp[0]);
            tempMaterial.SpecularColor.Y = stof(temp[1]);
            tempMaterial.SpecularColor.Z = stof(temp[2]);
        }
        else if (firstToken == "Ns")
            tempMaterial.SpecularExponent = stof(Tail(curline));
        else if (firstToken == "Ni")
            tempMaterial.OpticalDensity = stof(Tail(curline));
        else if (firstToken == "d")
            tempMaterial.Dissolve = stof(Tail(curline));
        else if (firstToken == "illum")
            tempMaterial.Illumination = stof(Tail(curline));
        else if (firstToken == "map_Ka")
            tempMaterial.AmbientTextureMap = Tail(curline);
        else if (firstToken == "map_Kd")
            tempMaterial.DiffuseTextureMap = Tail(curline);
        else if (firstToken == "map_Ns")
            tempMaterial.SpecularHightlightMap = Tail(curline);
        else if (firstToken == "map_d")
            tempMaterial.AlphaTextureMap = Tail(curline);
        else if (firstToken == "map_Bump" || firstToken == "map_bump" || firstToken == "bump")
            tempMaterial.BumpMap = Tail(curline);
    }

    LoadedMaterials.push_back(tempMaterial);

    if (LoadedMaterials.empty())
        return false;
    else
        return true;
}