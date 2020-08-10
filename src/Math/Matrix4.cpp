#include "Matrix4.hpp"
#include "Matrix3.hpp"

Matrix4::Matrix4() {
    
    col0 = Vector4(1, 0, 0, 0);
    col1 = Vector4(0, 1, 0, 0);
    col2 = Vector4(0, 0, 1, 0);
    col3 = Vector4(0, 0, 0, 1);
}

Matrix4::Matrix4(float V) {
    
    col0 = Vector4(V, 0, 0, 0);
    col1 = Vector4(0, V, 0, 0);
    col2 = Vector4(0, 0, V, 0);
    col3 = Vector4(0, 0, 0, V);
}

Matrix4::Matrix4(Matrix3 V) {

    col0 = Vector4(V.col0 , 0);
    col1 = Vector4(V.col1 , 0);
    col2 = Vector4(V.col2 , 0);
    col3 = Vector4(0, 0, 0, 1);
}

Matrix4::Matrix4(Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3) {

    col0 = c0;
    col1 = c1;
    col2 = c2;
    col3 = c3;
}

Matrix4 Matrix4::Inverse(Matrix4 V) {

    float Coef00 = V.col2.Z * V.col3.W - V.col3.Z * V.col2.W;
    float Coef02 = V.col1.Z * V.col3.W - V.col3.Z * V.col1.W;
    float Coef03 = V.col1.Z * V.col2.W - V.col2.Z * V.col1.W;

    float Coef04 = V.col2.Y * V.col3.W - V.col3.Y * V.col2.W;
    float Coef06 = V.col1.Y * V.col3.W - V.col3.Y * V.col1.W;
    float Coef07 = V.col1.Y * V.col2.W - V.col2.Y * V.col1.W;

    float Coef08 = V.col2.Y * V.col3.Z - V.col3.Y * V.col2.Z;
    float Coef10 = V.col1.Y * V.col3.Z - V.col3.Y * V.col1.Z;
    float Coef11 = V.col1.Y * V.col2.Z - V.col2.Y * V.col1.Z;

    float Coef12 = V.col2.X * V.col3.W - V.col3.X * V.col2.W;
    float Coef14 = V.col1.X * V.col3.W - V.col3.X * V.col1.W;
    float Coef15 = V.col1.X * V.col2.W - V.col2.X * V.col1.W;

    float Coef16 = V.col2.X * V.col3.Z - V.col3.X * V.col2.Z;
    float Coef18 = V.col1.X * V.col3.Z - V.col3.X * V.col1.Z;
    float Coef19 = V.col1.X * V.col2.Z - V.col2.X * V.col1.Z;

    float Coef20 = V.col2.X * V.col3.Y - V.col3.X * V.col2.Y;
    float Coef22 = V.col1.X * V.col3.Y - V.col3.X * V.col1.Y;
    float Coef23 = V.col1.X * V.col2.Y - V.col2.X * V.col1.Y;

    Vector4 Fac0(Coef00, Coef00, Coef02, Coef03);
    Vector4 Fac1(Coef04, Coef04, Coef06, Coef07);
    Vector4 Fac2(Coef08, Coef08, Coef10, Coef11);
    Vector4 Fac3(Coef12, Coef12, Coef14, Coef15);
    Vector4 Fac4(Coef16, Coef16, Coef18, Coef19);
    Vector4 Fac5(Coef20, Coef20, Coef22, Coef23);

    Vector4 Vec0(V.col1.X, V.col0.X, V.col0.X, V.col0.X);
    Vector4 Vec1(V.col1.Y, V.col0.Y, V.col0.Y, V.col0.Y);
    Vector4 Vec2(V.col1.Z, V.col0.Z, V.col0.Z, V.col0.Z);
    Vector4 Vec3(V.col1.W, V.col0.W, V.col0.W, V.col0.W);

    Vector4 Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
    Vector4 Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
    Vector4 Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
    Vector4 Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

    Vector4 SignA(+1, -1, +1, -1);
    Vector4 SignB(-1, +1, -1, +1);
    Matrix4 Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

    Vector4 Row0(Inverse.col0.X, Inverse.col1.X, Inverse.col2.X, Inverse.col3.X);

    Vector4 Dot0(V.col0 * Row0);
    
    float Dot1 = (Dot0.X + Dot0.Y) + (Dot0.Z + Dot0.W);
    float OOD = 1.0f / Dot1;
    return Inverse * OOD;
}

Matrix4 Matrix4::Translate(Vector3 translation) {

    return Translate(Matrix4(1), translation);
}

Matrix4 Matrix4::Translate(Matrix4 Target, Vector3 translation) {

    Matrix4 Result(Target);

    Result.col3 = Target.col0 * translation.X + Target.col1 * translation.Y + Target.col2 * translation.Z + Target.col3;
    return Result;
}

Matrix4 Matrix4::Scale(Vector3 translation) {

    return Scale(Matrix4(1), translation);
}

Matrix4 Matrix4::Scale(Matrix4 Target, Vector3 scale) {
    
    Matrix4 Result(Target);
    
    Result.col0 *= scale.X;
    Result.col1 *= scale.Y;
    Result.col2 *= scale.Z;
    
    return Result;
}

Matrix4 Matrix4::LookAtLH(Vector3 Eye, Vector3 Center, Vector3 Up)  {

    Vector3 f = Normalize(Center - Eye);
    Vector3 s = Normalize(Cross(Up, f));
    Vector3 u = Cross(f, s);

    Matrix4 Result = Matrix4(1);
    Result.col0.X = s.X;
    Result.col1.X = s.Y;
    Result.col2.X = s.Z;
    Result.col0.Y = u.X;
    Result.col1.Y = u.Y;
    Result.col2.Y = u.Z;
    Result.col0.Z = f.X;
    Result.col1.Z = f.Y;
    Result.col2.Z = f.Z;
    Result.col3.X = -Dot(s, Eye);
    Result.col3.Y = -Dot(u, Eye);
    Result.col3.Z = -Dot(f, Eye);

    return Result;
}

Matrix4 Matrix4::PerspectiveFovLH(float Fov, float Width, float Height, float zNear, float zFar) {

    if (Width < 0) {
        Debug::Alert("PespectiveFovLH Faild Because Width Is Negative");
        return Matrix4(1);
    }
    else if(Height < 0) {
        Debug::Alert("PespectiveFovLH Faild Because Height Is Negative");
        return Matrix4(1);
    }
    else if(Fov < 0) {
        Debug::Alert("PespectiveFovLH Faild Because Fov Is Negative");
        return Matrix4(1);
    }

    float rad = Fov;
    float h = cosf(0.5f * rad) / sinf(0.5f * rad);
    float w = h * Height / Width;

    Matrix4 Result(0);
    Result.col0.X = w;
    Result.col1.Y = h;
    Result.col2.Z = (zFar + zNear) / (zFar - zNear);
    Result.col2.W = 1.0f;
    Result.col3.Z = - (2.0f * zFar * zNear) / (zFar - zNear);
    return Result;
}

Matrix4 Matrix4::Ortho(float Left, float Right, float Bottom, float Top) {
    
    Matrix4 Result = Matrix4(1);
    Result.col0.X = 2.0f / (Right - Left);
    Result.col1.Y = 2.0f / (Top - Bottom);
    Result.col2.Z = - 1.0f;
    Result.col3.X = - (Right + Left) / (Right - Left);
    Result.col3.Y = - (Top + Bottom) / (Top - Bottom);
    
    return Result;
}

Matrix4 Matrix4::CreateFromAxisAngle(Vector3 Axis, float Angle) {
    Vector3 NormAxis = Normalize(Axis);
    float axisX = NormAxis.X;
    float axisY = NormAxis.Y;
    float axisZ = NormAxis.Z;

    float Cos = cos(-Angle);
    float Sin = sin(-Angle);
    float t = 1.0f - Cos;

    float tXX = t * axisX * axisX;
    float tXY = t * axisX * axisY;
    float tXZ = t * axisX * axisZ;
    float tYY = t * axisY * axisY;
    float tYZ = t * axisY * axisZ;
    float tZZ = t * axisZ * axisZ;
    
    float sinX = Sin * axisX;
    float sinY = Sin * axisY;
    float sinZ = Sin * axisZ;

    Matrix4 Result = Matrix4();
    Result.col0.X = tXX + Cos;
    Result.col0.Y = tXY - sinZ;
    Result.col0.Z = tXZ + sinY;
    Result.col0.W = 0;
    Result.col1.X = tXY + sinZ;
    Result.col1.Y = tYY + Cos;
    Result.col1.Z = tYZ - sinX;
    Result.col1.W = 0;
    Result.col2.X = tXZ - sinY;
    Result.col2.Y = tYZ + sinX;
    Result.col2.Z = tZZ + Cos;
    Result.col2.W = 0;
    Result.col3 = Vector4(0, 0, 0, 1);

    return Result;
}

Matrix4 Matrix4::FromQuaternion(Quaternion quat) {
    Vector4 AxisAngle = Quaternion::ToAxisAngle(quat);
    return CreateFromAxisAngle(AxisAngle.Xyz(), AxisAngle.W);
}

Matrix4 Matrix4::operator*(const Matrix4& right) const {

    Vector4 SrcA0 = col0;
    Vector4 SrcA1 = col1;
    Vector4 SrcA2 = col2;
    Vector4 SrcA3 = col3;

    Vector4 SrcB0 = right.col0;
    Vector4 SrcB1 = right.col1;
    Vector4 SrcB2 = right.col2;
    Vector4 SrcB3 = right.col3;

    Matrix4 Result(0);
    Result.col0 = SrcA0 * SrcB0.X + SrcA1 * SrcB0.Y + SrcA2 * SrcB0.Z + SrcA3 * SrcB0.W;
    Result.col1 = SrcA0 * SrcB1.X + SrcA1 * SrcB1.Y + SrcA2 * SrcB1.Z + SrcA3 * SrcB1.W;
    Result.col2 = SrcA0 * SrcB2.X + SrcA1 * SrcB2.Y + SrcA2 * SrcB2.Z + SrcA3 * SrcB2.W;
    Result.col3 = SrcA0 * SrcB3.X + SrcA1 * SrcB3.Y + SrcA2 * SrcB3.Z + SrcA3 * SrcB3.W;
    return Result;
}

Matrix4 Matrix4::operator*(const float& right) const {

    return Matrix4(
        col0 * right,
        col1 * right,
        col2 * right,
        col3 * right
    );
}

Vector4 Matrix4::operator*(const Vector4& right) const {

    Vector4 Mov0(right.X);
    Vector4 Mov1(right.Y);
    Vector4 Mul0 = col0 * Mov0;
    Vector4 Mul1 = col1 * Mov1;
    Vector4 Add0 = Mul0 + Mul1;

    Vector4 Mov2(right.Z);
    Vector4 Mov3(right.W);
    Vector4 Mul2 = col2 * Mov2;
    Vector4 Mul3 = col3 * Mov3;
    Vector4 Add1 = Mul2 + Mul3;
    return Add0 + Add1;

}

Vector3 Matrix4::operator*(const Vector3& right) const {
    return (*this * Vector4(right)).Xyz();
}