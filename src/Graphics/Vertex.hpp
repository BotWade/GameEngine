#ifndef _VERTEX_
#define _VERTEX_

#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"

struct Vertex {

    Vector3 Position;
    Vector3 Normal;
    Vector2 TextureCoordinate;
};

#endif