#ifndef _ASTEROIDS_
#define _ASTEROIDS_

#include "../SpaceObject.hpp"

class Asteroid : public SpaceObject {
public:    
    void Load() override;
};

#endif