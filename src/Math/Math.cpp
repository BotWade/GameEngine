#include "Math.hpp"
#include "../Core/Window.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "../Core/Camera.hpp"

unsigned short getShort(unsigned char* array, int offset) {
    
    return (short)(((short)array[offset]) << 8) | array[offset + 1];
}

unsigned int getInt32(unsigned char* array, int offset) {
    
    return int((unsigned char)(array[offset])     << 24 |
               (unsigned char)(array[offset + 1]) << 16 |
               (unsigned char)(array[offset + 2]) << 8 |
               (unsigned char)(array[offset + 3]));
}

bool is_number(const string& s) {
    
    return !s.empty() && find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

Vector3 Cross(const Vector3 Left, const Vector3 Right) {

    return Vector3(Left.Y * Right.Z - Right.Y * Left.Z,
                   Left.Z * Right.X - Right.Z * Left.X,
                   Left.X * Right.Y - Right.X * Left.Y);
}

Vector3 Projection(const Vector3 Left, const Vector3 Right) {

    Vector3 Rightn = Right / Magnitude(Right);
    return (Rightn) * Dot(Left, Rightn);
}

Vector3 Normalize(const Vector3 Input) {

    return Input * ReverseSqrt(Dot(Input, Input));
}

Vector3 RotateX(Vector3 Origin, Vector3 Point, float Deg) {

    Vector3 x = Vector3(1, 0, 0);
    Vector3 y = Vector3(0, cos(Deg), sin(Deg));
    Vector3 z = Vector3(0, -sin(Deg), cos(Deg));

    return Vector3(
        Point.X * x.X + Point.Y * y.X + Point.Z * z.X + Origin.X,
        Point.X * x.Y + Point.Y * y.Y + Point.Z * z.Y + Origin.Y,
        Point.X * x.Z + Point.Y * y.Z + Point.Z * z.Z + Origin.Z
    );
}

Vector3 RotateY(Vector3 Origin, Vector3 Point, float Deg) {

    Vector3 x = Vector3(cos(Deg), 0, sin(Deg));
    Vector3 y = Vector3(0, 1, 0);
    Vector3 z = Vector3(-sin(Deg), 0, cos(Deg));

    return Vector3(
        Point.X * x.X + Point.Y * y.X + Point.Z * z.X + Origin.X,
        Point.X * x.Y + Point.Y * y.Y + Point.Z * z.Y + Origin.Y,
        Point.X * x.Z + Point.Y * y.Z + Point.Z * z.Z + Origin.Z
    );
}

Vector3 RotateZ(Vector3 Origin, Vector3 Point, float Deg) {

    Vector3 x = Vector3(cos(Deg), sin(Deg), 0);
    Vector3 y = Vector3(-sin(Deg), cos(Deg), 0);
    Vector3 z = Vector3(0, 0, 1);

    return Vector3(
        Point.X * x.X + Point.Y * y.X + Point.Z * z.X + Origin.X,
        Point.X * x.Y + Point.Y * y.Y + Point.Z * z.Y + Origin.Y,
        Point.X * x.Z + Point.Y * y.Z + Point.Z * z.Z + Origin.Z
    );
}

Vector3 Screen2World(Vector2 Screen) {
    Vector2 Size = Window::GetSize();
    Vector3 ray_nds = Vector3((2.0f * Screen.X) / Size.X - 1.0f, 1.0f - (2.0f * Screen.Y) / Size.Y, 1.0f);
    Vector4 Ray_Clip = Vector4(ray_nds.Xy(), 1.0, 1.0);
    Vector4 Ray_eye = Vector4((Matrix4::Inverse(Camera::Projection) * Ray_Clip).Xy(), 1.0, 0.0);
    return Normalize((Matrix4::Inverse(Camera::View) * Ray_eye).Xyz());
}

Vector3 Abs(Vector3 Input) {
    return Vector3(abs(Input.X), abs(Input.Y), abs(Input.Z));
}

float MaxDim(Vector3 Input) {
    return (Input.X > Input.Y && Input.X > Input.Z ? Input.X : (Input.Y > Input.Z ? Input.Y : Input.Z));
}

float Dot(const Vector3 Left, const Vector3 Right) {

    return Left.X * Right.X + Left.Y * Right.Y + Left.Z * Right.Z;
}

float AngleBetween(const Vector3 Left, const Vector3 Right) {

    return acosf((Dot(Left, Right)) / (Magnitude(Left) * Magnitude(Right)));
}

float Magnitude(const Vector3 Input) {

    return (sqrtf(powf(Input.X, 2) + powf(Input.Y, 2) + powf(Input.Z, 2)));
}

float Radians(float Degrees) {
    return Degrees * 0.01745329251994329576923690768489f;
}

float ReverseSqrt(float Input) {
    return 1.0f / sqrt(Input);
}

float Mix(Vector3 Input) {
    return Input.X * (1.0f - Input.Z) + Input.Y * Input.Z;
}

float Lerp(Vector3 Input) {
    return Mix(Input);
}

Vector3 operator*(const float& left, const Vector3& right) {
    
    return Vector3(right.X * left, right.Y * left, right.Z * left);
}