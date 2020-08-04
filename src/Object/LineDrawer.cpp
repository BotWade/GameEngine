#include "LineDrawer.hpp"

LineDrawer::LineDrawer(Vector3 Origin, Vector3 End) {
    origin = Origin;
    end = End;
}

void LineDrawer::Render() {

    Renderer::DrawLine(origin, end);
}
