#ifndef _FRAME_
#define _FRAME_

#include <vector>

#include "Color.hpp"

using namespace std;

struct Frame {

    unsigned int TextureId;

    vector<Color> TextureData;
    vector<unsigned char> Data;

    Frame();
};


#endif