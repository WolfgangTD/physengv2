#include <cmath>
#pragma once

struct Vec3
{
    float x=0;
    float y=0;
    float z=0;
};

Vec3 getVecPos (Vec3 a){
    return Vec3 {a.x, a.y, a.z};
}

float distance( Vec3 a, Vec3 b )
{
  return sqrt((a.x - b.x)) + sqrt((a.y - b.y));
}

float dotProduct (Vec3 a, Vec3 b){
    float first = a.x * b.x;
    float second = a.y * b.y;
    float third = a.z * b.z;

    return first + second + third;
}