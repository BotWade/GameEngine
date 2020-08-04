#include "Algorithm.hpp"

bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b) {

    Vector3 BA = b - a;
    return Dot(Cross(BA, p1 - a), Cross(BA, p2 - a)) >= 0;
}

Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3) {

    Vector3 u = t2 - t1;
    Vector3 v = t3 - t1;
    return Cross(u, v);
}

bool InTriangle(Vector3 point, Vector3 ta, Vector3 tb, Vector3 tc) {

    //Check if it is within an infinite prism that the triangle outlines, if it isnt its impossible to be inside the triangle
    if (!(SameSide(point, ta, tb, tc) & SameSide(point, tb, ta, tc) && SameSide(point, tc, ta, tb)))
        return false;

    // Calculate Triangles Normal -> Project the point into a normal -> if the distance from the triangle to the point is 0, it lies on the triangle
    return Magnitude(Projection(point, GenTriNormal(ta, tb, tc))) == 0;
}

