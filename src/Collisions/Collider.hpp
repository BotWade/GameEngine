#ifndef _COLLIDER_
#define _COLLIDER_

#include <vector>

#include "../Object/Component.hpp"
#include "../Math/Triangle.hpp"
#include "../Math/Ray.hpp"
#include "../Math/Transform.hpp"
#include "../Mesh/Mesh.hpp"

using namespace std;

class Collider : public Component {
public:
    vector<Triangle> Triangles;

    void LoadCollider(Mesh *mesh);
    virtual bool CheckCollision(Ray ray, bool useDoublePercision = false, Vector3* hit = nullptr);
};

#endif