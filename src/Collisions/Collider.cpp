#include "Collider.hpp"
#include "../Core/ObjectsHandler.hpp"
#include "../Object/RayDrawer.hpp"

#define BACKFACE_CULLING

void Collider::LoadCollider(Mesh *mesh) {

    int triCount = mesh->Indices.size();
    
    for (int Index = 0; Index < triCount; Index +=3) {

        Triangle triangle = Triangle(mesh->Data[mesh->Indices[Index]].Position,
                                     mesh->Data[mesh->Indices[Index + 1]].Position,
                                     mesh->Data[mesh->Indices[Index + 2]].Position);
        
        triangle.Edge1 = triangle.Pos2 - triangle.Pos1;
        triangle.Edge2 = triangle.Pos3 - triangle.Pos1;

        Triangles.push_back(triangle);
    }
}

///Check If Rays Collides
///Wateright intersection algorithm
///ATTENTION! Default Collision Check Checks Each Triangle Against Ray!
///If You Are Going To Use Default Collision Check DONT Use A Mesh As A Collider (1000 triangles == 1000 Checks)
///Only Use Double Percision If Really Needed
bool Collider::CheckCollision(Ray ray, bool useDoublePercision) {

    if (Triangles.size() == 0)
        return false;

    int Size = Triangles.size();

    int Kz = MaxDim(Abs(ray.Direction));
    int Kx = (Kz + 1.0f == 3 ? 0 : Kz + 1.0f);
    int Ky = (Kx + 1.0f == 3 ? 0 : Kx + 1.0f);

    if (ray.Direction[Kz] < 0.0f) {
        float temp = Kx;
        Kx = Ky;
        Ky = temp;
    }

    float Sx = ray.Direction[Kx] / ray.Direction[Kz];
    float Sy = ray.Direction[Ky] / ray.Direction[Kz];

    for (int Index = 0; Index < Size; Index++) {

        Vector3 A = Triangles[Index].Pos1 + object->transform.Position - ray.Origin;
        Vector3 B = Triangles[Index].Pos2 + object->transform.Position - ray.Origin;
        Vector3 C = Triangles[Index].Pos3 + object->transform.Position - ray.Origin;

        float Ax = A[Kx] - Sx * A[Kz];
        float Ay = A[Ky] - Sy * A[Kz];
        float Bx = B[Kx] - Sx * B[Kz];
        float By = B[Ky] - Sy * B[Kz];
        float Cx = C[Kx] - Sx * C[Kz];
        float Cy = C[Ky] - Sy * C[Kz];

        float U = Cx * By - Cy * Bx;
        float V = Ax * Cy - Ay * Cx;
        float W = Bx * Ay - By * Ax;

        ///
        /// Fallback Test Double Against Edges
        /// Using Double Percision 
        ///
        if ((U == 0.0f || V == 0.0f || W == 0.0f) && useDoublePercision) {

            double CxBy = (double)Cx * (double)By;
            double CyBx = (double)Cy * (double)Bx;
            U = (float)(CxBy - CyBx);

            double AxCy = (double)Ax * (double)Cy;
            double AyCx = (double)Ay * (double)Cx;
            V = (float)(AxCy - AyCx);

            double BxAy = (double)Bx * (double)Ay;
            double ByAx = (double)By * (double)Ax;
            W = (float)(BxAy - ByAx);
        }

        #ifdef BACKFACE_CULLING
            if  (U < 0.0f || V < 0.0f || W < 0.0f) continue;
        #else
            if ((U < 0.0f || V < 0.0f || W < 0.0f) && (U > 0.0f || V > 0.0f || W > 0.0f)) continue;
        #endif

        float det = U + V + W;

        if (det == 0.0f)
            continue;
            
        return true;
    }
    
    return false;
}