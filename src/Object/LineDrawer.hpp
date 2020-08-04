#ifndef _LINEDRAWER_
#define _LINEDRAWER_

#include "Component.hpp"
#include "../Math/Vector3.hpp"
#include "../Graphics/Renderer.hpp"

class LineDrawer : public Component {
public:
    Vector3 origin;
    Vector3 end;
    
    LineDrawer(Vector3 Origin, Vector3 End);
    
    void Render() override;
};


#endif