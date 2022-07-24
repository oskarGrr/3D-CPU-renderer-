#pragma once

//a 3d vector with a 4th homogenous w component.
//the multiplications with 4x4 matricies will use
//collumn major order not row major order.
//4x4 matrix times 4x1 vector = collumn major.
//1x4 vector times 4x4 matrix = row major
class vec4
{
public:
    float x,y,z,w;

    vec4(const float, const float, const float);
    vec4();

    vec4& operator*=(const vec4&);
    vec4& operator+=(const vec4&);
    vec4& operator/=(const float);
    vec4& operator-=(const vec4&);
    vec4 operator-(const vec4&) const;
    vec4 operator/(const float) const;  
    vec4 operator*(const float) const;
    vec4 operator+(const vec4&) const;

    float getMagnitude() const;//get the magnitude of *this
    static vec4 normalize(vec4&);
    static float dot(const vec4&, const vec4&);//dot product
    static vec4 cross(const vec4&, const vec4&);//cross product
};