#include "Quaternion.hpp"
#include "Math.hpp"
#include "Matrix3.hpp"
#include <math.h>

Quaternion::Quaternion() {
    Quat = Vector4(0, 0, 0, 1);
}

Quaternion::Quaternion(Vector4 quat) {
    Quat = quat;
}

Quaternion::Quaternion(Vector3 Xyz, float W) {
    Quat = Vector4(Xyz, W);
}


float Quaternion::Length() {
    return sqrt(Quat.W * Quat.W  + Quat.Xyz().LengthSquared());
}

Vector3 Quaternion::ToEuler() {

    Vector3 Euler;

    float sinr_cosp = 2 * (Quat.W * Quat.X + Quat.Y * Quat.Z);
    float consr_cosp = 1 - 2 * (Quat.X * Quat.X + Quat.Y * Quat.Y);
    Euler.X = atan2(sinr_cosp, consr_cosp);

    float sinp = 2 * (Quat.W * Quat.Y - Quat.Z * Quat.X);
    if (abs(sinp) >= 1)
        Euler.Y = copysign(M_PI / 2, sinp);
    else
        Euler.Y = asin(sinp);

    float siny_cosp = 2 * (Quat.W * Quat.Z + Quat.X * Quat.Y);
    float cosy_cosp = 1 - 2 * (Quat.Y * Quat.Y + Quat.Z * Quat.Z);
    Euler.Z = atan2(siny_cosp, cosy_cosp);

    return Euler;
}

Quaternion Quaternion::EulerToQuaternion(Vector3 Euler) {

    float cy = cos(Euler.Z * 0.5);
    float sy = sin(Euler.Z * 0.5);
    float cp = cos(Euler.Y * 0.5);
    float sp = sin(Euler.Y * 0.5);
    float cr = cos(Euler.X * 0.5);
    float sr = sin(Euler.X * 0.5);

    Vector4 q;
    q.W = cr * cp * cy + sr * sp * sy;
    q.X = sr * cp * cy - cr * sp * sy;
    q.Y = cr * sp * cy + sr * cp * sy;
    q.Z = cr * cp * sy - sr * sp * cy;

    return Quaternion(q);
}

Quaternion CreateFromAxisAngle(Vector3 axis, float angle) {
    float halfAngle = angle * .5f;
    float s = sin(halfAngle);
    Quaternion q;
    q.Quat.X = axis.X * s;
    q.Quat.Y = axis.Y * s;
    q.Quat.Z = axis.Z * s;
    q.Quat.W = cos(halfAngle);
    return q;
}

Quaternion Quaternion::LookRotation(Vector3 Source, Vector3 Destination) {

    Vector3 forwardVector = Normalize(Destination - Source);

    float dot = Dot(Vector3(0, 0, 1), forwardVector);

    if (abs(dot - (-1.0f)) < 0.000001f)
        return Quaternion(Vector4(0.0f , 1.0f, 0.0f, M_PI));
    if (abs(dot - (1.0f)) < 0.000001f)
        return Quaternion();

    float rotAngle = acos(dot);
    Vector3 rotAxis = Cross(Vector3(0, 0, 1), forwardVector);
    rotAxis = Normalize(rotAxis);
    return CreateFromAxisAngle(rotAxis, rotAngle);
}

Quaternion Quaternion::SlerpNotNormalized(Quaternion Left, Quaternion Right, float Slerp) {

    float RawCosom = Left.Quat.X * Right.Quat.X + Left.Quat.Y * Right.Quat.Y + Left.Quat.Z * Right.Quat.Z + Left.Quat.W * Right.Quat.W;
    float Cosom = RawCosom >= 0.f ? RawCosom : -RawCosom;

    float Scale0, Scale1;

    if (Cosom < 0.9999f) {

        float Omega = acos(Cosom);
        float InvSin = 1.f/sin(Omega);
        Scale0 = sin((1.f - Slerp) * Omega) * InvSin;
        Scale1 = sin(Slerp * Omega) * InvSin;
    }
    else {
        Scale0 = 1.0f - Slerp;
        Scale1 = Slerp;
    }

    Scale1 = RawCosom >= 0.f ? Scale1 : -Scale1;

    return Quaternion(Vector4(Scale0 * Left.Quat.X + Scale1 * Right.Quat.X,
                              Scale0 * Left.Quat.Y + Scale1 * Right.Quat.Y,
                              Scale0 * Left.Quat.Z + Scale1 * Right.Quat.Z,
                              Scale0 * Left.Quat.W + Scale1 * Right.Quat.W));
}

Quaternion Quaternion::Slerp(Quaternion Left, Quaternion Right, float Slerp) {
    return Normalize(SlerpNotNormalized(Left, Right, Slerp));
}

Quaternion Quaternion::lerp(Quaternion Left, Quaternion Right, float Time) {

    return Left * (1.0f - Time) + (Right * Time);
}

Quaternion Quaternion::FromAxisAngle(Vector3 Axis, float Angle) {
    if (Axis.LengthSquared() == 0.0f)
        return Quaternion();

    Angle *= 0.5f;
    return Normalize(Quaternion(Normalize(Axis) * sin(Angle), cos(Angle)));
}

Vector4 Quaternion::ToAxisAngle(Quaternion quat) {
    
    Quaternion q = quat;
    if (abs(quat.Quat.W) > 1.0f)
        q = Normalize(q);

    Vector4 Result = Vector4(0.0f, 0.0f, 0.0f, 2.0f * acos(q.Quat.W));
    
    float den = sqrt(1.0f - (q.Quat.W * q.Quat.W));

    if (den > 0.0001f)
        Result.Xyz(q.Quat.Xyz() / den);
    else
        Result.Xyz(Vector3(1.0f, 0.0f, 0.0f));

    return Result;
}

Quaternion Quaternion::operator*(float right) {

    return Quaternion(Vector4(
        Quat.X * right,
        Quat.Y * right,
        Quat.Z * right,
        Quat.W * right
    )); 
}

Quaternion Quaternion::operator*(Quaternion right) {

    return Quaternion(Vector4(
        Quat.W * right.Quat.X + Quat.X * right.Quat.W + Quat.Y * right.Quat.Z - Quat.Z * right.Quat.Y,
        Quat.W * right.Quat.Y + Quat.Y * right.Quat.W + Quat.Z * right.Quat.X - Quat.X * right.Quat.Z,
        Quat.W * right.Quat.Z + Quat.Z * right.Quat.W + Quat.X * right.Quat.Y - Quat.Y * right.Quat.X,
        Quat.W * right.Quat.W - Quat.X * right.Quat.X - Quat.Y * right.Quat.Y - Quat.Z * right.Quat.Z
    ));
}

Quaternion Quaternion::operator+(Quaternion right) {

    return Quaternion(Vector4(
        Quat.X + right.Quat.X, 
        Quat.Y + right.Quat.Y,
        Quat.Z + right.Quat.Z,
        Quat.W + right.Quat.W));
}

Vector3 Quaternion::operator*(Vector3 right) {

    Vector3 Xyz = Quat.Xyz();
    Vector3 Uv = Cross(Xyz, right);
    Vector3 Uuv = Cross(Xyz, Uv);

    return right + ((Uv * Quat.W) + Uuv) * 2.0f;
}