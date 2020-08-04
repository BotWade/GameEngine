#ifndef _MATH_
#define _MATH_

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <algorithm>

#include "Vector2.hpp"
#include "Vector3.hpp"

using namespace std;

unsigned short getShort(unsigned char* array, int offset);
unsigned int getInt32(unsigned char* array, int offset);
bool is_number(const string& s);

Vector3 Cross(const Vector3 Left, const Vector3 Right);
Vector3 Projection(const Vector3 Left, const Vector3 Right);
Vector3 Normalize(const Vector3 Input);
Vector3 RotateX(Vector3 Origin, Vector3 Point, float Deg);
Vector3 RotateY(Vector3 Origin, Vector3 Point, float Deg);
Vector3 RotateZ(Vector3 Origin, Vector3 Point, float Deg);
Vector3 Screen2World(Vector2 Screen);
Vector3 Abs(Vector3 Input);
float MaxDim(Vector3 Input);

float Dot(const Vector3 Left, const Vector3 Right);
float AngleBetween(const Vector3 Left, const Vector3 Right);
float Magnitude(const Vector3 Input);
float Radians(float Degrees);
float ReverseSqrt(float Input);
float Lerp(Vector3 Input);
float Mix(Vector3 Input);

Vector3 operator*(const float& left, const Vector3& right);

#endif