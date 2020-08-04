#ifndef _ALGORITHM_
#define _ALGORITHM_

#include "../../Math/Math.hpp"

bool SameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b);
Vector3 GenTriNormal(Vector3 t1, Vector3 t2, Vector3 t3);
bool InTriangle(Vector3 point, Vector3 ta, Vector3 tb, Vector3 tc);


#endif