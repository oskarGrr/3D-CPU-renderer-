#include "mat4.h"
#include "vec4.h"
#include <cmath>//trig functions
#define PI 3.14159f

//matrix multiplication (its m0 * m1)
mat4 mat4::matMul(const mat4& m0, const mat4& m1)
{
    mat4 result;

    for(int i = 0; i < 4; ++i)
        for(int j = 0; j < 4; ++j)
            for(int k = 0; k < 4; ++k)
                result.mat[i][j] += m0.mat[i][k] * m1.mat[k][j];
        
    return result;
}

mat4 mat4::createProjectionMatrix(const float fov, 
                                  const float zFar, 
                                  const float zNear,
                                  const float aspectRatio)
{
    mat4 mat;

    float fovRadians = 1.0f/tanf(fov * 0.5f / 180.0f * PI);

    mat.mat[0][0] = aspectRatio * fovRadians;
    mat.mat[1][1] = fovRadians;
    mat.mat[2][2] = zFar / (zFar - zNear);
    mat.mat[3][2] = (-zFar * zNear) / (zFar - zNear);
    mat.mat[2][3] = 1.0f;
    mat.mat[3][3] = 0.0f;

    return mat;
}

//4x4 matrix * 4x1 vector -> transformed 4x1 vector (collumn major)
vec4 mat4::mat4MulByVector4(const vec4& in, const mat4& m)
{
    vec4 out;
    out.x = in.x*m.mat[0][0] + in.y*m.mat[1][0] + in.z*m.mat[2][0] + in.w*m.mat[3][0];
    out.y = in.x*m.mat[0][1] + in.y*m.mat[1][1] + in.z*m.mat[2][1] + in.w*m.mat[3][1];
    out.z = in.x*m.mat[0][2] + in.y*m.mat[1][2] + in.z*m.mat[2][2] + in.w*m.mat[3][2];
    out.w = in.x*m.mat[0][3] + in.y*m.mat[1][3] + in.z*m.mat[2][3] + in.w*m.mat[3][3];
    return out;
}

mat4 mat4::zRotationMatrix(const float radians)
{
    mat4 result;
    result.mat[0][0] = cosf(radians);
    result.mat[0][1] = sinf(radians);
    result.mat[1][0] = -sinf(radians);
    result.mat[1][1] = cosf(radians);
    result.mat[2][2] = 1.0f;
    result.mat[3][3] = 1.0f;
    return result;
}

mat4 mat4::xRotationMatrix(const float radians)
{
    mat4 result;
    result.mat[0][0] = 1.0f;
    result.mat[1][1] = cosf(radians);
    result.mat[1][2] = sinf(radians);
    result.mat[2][1] = -sinf(radians);
    result.mat[2][2] = cosf(radians);
    result.mat[3][3] = 1.0f;
    return result;
}

mat4 mat4::yRotationMatrix(const float radians)
{
    mat4 result;
    result.mat[0][0] = cosf(radians);
    result.mat[2][0] = sinf(radians);
    result.mat[1][1] = 1.0f;
    result.mat[0][2] = -sinf(radians);
    result.mat[2][2] = cosf(radians);
    result.mat[3][3] = 1.0f;
    return result;
}

mat4 mat4::translationMatrix(const float x, const float y, const float z)
{
    mat4 result;
    result.mat[0][0] = 1.0f;
    result.mat[1][1] = 1.0f;
    result.mat[2][2] = 1.0f;
    result.mat[3][3] = 1.0f;
    result.mat[3][0] = x;
    result.mat[3][1] = y;
    result.mat[3][2] = z;
    return result;
}

mat4 mat4::identityMatrix()
{
    mat4 result;
    result.mat[0][0] = 1.0f;
    result.mat[1][1] = 1.0f;
    result.mat[2][2] = 1.0f;
    result.mat[3][3] = 1.0f;
    return result;
}