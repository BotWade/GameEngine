#ifndef _CAMERA_
#define _CAMERA_

#include "Window.hpp"
#include "../Math/Math.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Quaternion.hpp"

class Camera {
public:
    static Vector3 Position;
    static Vector3 Center;
    static Vector3 Up;
    static Quaternion Rotation;

    static Matrix4 Projection;
    static Matrix4 View;
    static Matrix4 ProjectionView;
    
    static bool FirstPersonMode;
    
    static void Update();
};

#endif