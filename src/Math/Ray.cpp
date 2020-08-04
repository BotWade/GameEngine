#include "Ray.hpp"

Ray::Ray() {
    Origin = Direction = Vector3();
    StopFirst = false;
}

Ray::Ray(Vector3 origin, Vector3 direction) {

    Origin = origin;
    Direction = direction;
    StopFirst = false;
}

Vector3 Ray::GetPoint(float Distance) {
    return Origin + Direction * Distance;
}

bool Ray::operator==(const Ray& other) const {
    return Origin == other.Origin && Direction == other.Direction && StopFirst == other.StopFirst;
}

bool Ray::operator<(const Ray& other) const {
    return (Origin.X < other.Origin.X || Origin.Y < other.Origin.Y || Origin.Z < other.Origin.Z) && (Direction.X < other.Direction.X || Direction.Y < other.Direction.Y || Direction.Z < other.Direction.Z);
}