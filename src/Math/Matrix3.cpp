#include "Matrix3.hpp"

Matrix3::Matrix3(Matrix4 v) {
    col0 = v.col0.Xyz();
    col1 = v.col1.Xyz();
    col2 = v.col2.Xyz();
}

Matrix3::Matrix3(Quaternion Quat) {

    float qxx = Quat.Quat.X * Quat.Quat.X;
    float qyy = Quat.Quat.Y * Quat.Quat.Y;
    float qzz = Quat.Quat.Z * Quat.Quat.Z;
    float qxz = Quat.Quat.X * Quat.Quat.Z;
    float qxy = Quat.Quat.X * Quat.Quat.Y;
    float qyz = Quat.Quat.Y * Quat.Quat.Z;
    float qwx = Quat.Quat.W * Quat.Quat.X;
    float qwy = Quat.Quat.W * Quat.Quat.Y;
    float qwz = Quat.Quat.W * Quat.Quat.Z;

    col0.X = 1.0f - 2.0f * (qyy + qzz);
    col0.Y = 2.0f * (qxy + qwz);
    col0.Z = 2.0f * (qxz + qwy);

    col1.X = 2.0f * (qxy + qwz);
    col1.Y = 1.0f - 2.0f * (qxx + qzz);
    col1.Z = 2.0f * (qyz + qwx);
    
    col2.X = 2.0f * (qxz + qwy);
    col2.Y = 2.0f * (qyz - qwx);
    col2.Z = 1.0f - 2.0f * (qxx + qyy);
}