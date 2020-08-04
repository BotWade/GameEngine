#include "RayDrawer.hpp"

RayDrawer::RayDrawer(Ray newRay) {
    ray = newRay;
}

void RayDrawer::Render() {

    Renderer::DrawRay(ray);
}
