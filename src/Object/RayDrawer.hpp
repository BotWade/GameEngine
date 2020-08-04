#ifndef _RAYDRAWER_
#define _RAYDRAWER_


#include "Component.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Ray.hpp"
#include "../Graphics/Renderer.hpp"

class RayDrawer : public Component {
public:
    Ray ray;
    
    RayDrawer(Ray newRay);
    
    void Render() override;
};

#endif