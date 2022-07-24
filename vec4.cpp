#include "vec4.h"
#include <cmath>

vec4::vec4(const float x, const float y, const float z)
    : x(x), y(y), z(z), w(1.0f)
{}

vec4::vec4() 
    : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{}

vec4& vec4::operator+=(const vec4& rhs)
{
    x = x+rhs.x;
    y = y+rhs.y;
    z = z+rhs.z;
    return *this;
}

vec4& vec4::operator*=(const vec4& rhs)
{
    x = x*rhs.x;
    y = y*rhs.y;
    z = z*rhs.z;
    return *this;
}

vec4 vec4::operator*(const float scaler) const
{
    return {x*scaler, y*scaler, z*scaler};
}

vec4 vec4::operator-(const vec4& rhs) const
{
    return {x-rhs.x, y-rhs.y, z-rhs.z};
}

vec4& vec4::operator/=(const float d)
{
    x = x/d;
    y = y/d;
    z = z/d;
    return *this;
}

vec4& vec4::operator-=(const vec4& rhs)
{
    x = x-rhs.x;
    y = y-rhs.y;
    z = z-rhs.z;
    return *this;
}

vec4 vec4::operator/(const float d) const
{
    return {x/d, y/d, z/d};
}

vec4 vec4::operator+(const vec4& rhs) const
{
    return {x+rhs.x, y+rhs.y, z+rhs.z};
}

vec4 vec4::normalize(vec4& v)
{
    vec4 newVec = v;
    float magnitude;
    magnitude = v.getMagnitude();
    newVec /= magnitude;
    return newVec;
}

float vec4::getMagnitude() const
{
    return sqrtf(x*x + y*y + z*z);
}

float vec4::dot(const vec4& v0, const vec4& v1)
{
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

vec4 vec4::cross(const vec4& v0, const vec4& v1)
{
    return {
            v0.y*v1.z - v0.z*v1.y,
            v0.z*v1.x - v0.x*v1.z,
            v0.x*v1.y - v0.y*v1.x
           };
}