#ifndef _MATERIAL_
#define _MATERIAL_

#include <string>

#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"

using namespace std;

struct Material {
    
    Material();

    string Name;
    Vector3 AmbientColor;
    Vector3 DiffuseColor;
    Vector3 SpecularColor;
    float SpecularExponent;
    float OpticalDensity;
    float Dissolve;
    int Illumination;
    string AmbientTextureMap;
    string DiffuseTextureMap;
    string SpecularTextureMap;
    string SpecularHightlightMap;
    string AlphaTextureMap;
    string BumpMap;
};


#endif