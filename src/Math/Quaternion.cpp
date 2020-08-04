#include "Quaternion.hpp"
#include "Math.hpp"

Quaternion::Quaternion() {
    Quat = Vector4(0, 0, 0, 1);
}

Quaternion::Quaternion(Vector4 quat) {
    Quat = quat;
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

Quaternion Quaternion::lerp(Quaternion Left, Quaternion Right, float Time) {

    return Left * (1.0f - Time) + (Right * Time);
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