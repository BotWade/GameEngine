#ifndef _IMAGE_
#define _IMAGE_

#include "../InterfaceObject.hpp"
#include "../../Core/TextureManager.hpp"
class Image : public InterfaceObject {
public:
    
    Texture* texture;
    virtual void Load() override;
    virtual void Render() override;
};

#endif