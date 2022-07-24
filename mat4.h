#pragma once
#include "vec4.h"

//a 4x4 matrix class.
//the multiplications with 4x1 vectors will use
//collumn major order not row major order.
//4x4 matrix times 4x1 vector = collumn major.
//1x4 vector times 4x4 matrix = row major
class mat4
{
public:
    float mat[4][4]={};

    //matrix multiplication (its m0 * m1)
    static mat4 matMul(const mat4& m0, const mat4& m1);
    static mat4 createProjectionMatrix(const float fov,
                                       const float zFar,
                                       const float zNear,
                                       const float aspectRatio);

    //4x4 matrix * 4x1 vector -> transformed 4x1 vector
    static vec4 mat4MulByVector4(const vec4& in, const mat4& m);
    mat4 identityMatrix();

    //transformation matricies
    static mat4 zRotationMatrix(const float radians);
    static mat4 xRotationMatrix(const float radians);
    static mat4 yRotationMatrix(const float radians);
    static mat4 translationMatrix(const float x, const float y, const float z);
};