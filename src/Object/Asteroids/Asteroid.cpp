#include "Asteroid.hpp"

void Asteroid::Load() {

    LoadObject("Data/Shaders/Basic.shader", "Data/Meshes/Asteroids.obj");

    object->transform.localPosition = Vector3(rand() % (100 - (-100) + 1) + -100, rand() % (100 - (-100) + 1) + -100, rand() % (100 - (-100) + 1) + -100);

    LoadCollider(mesh);
}
