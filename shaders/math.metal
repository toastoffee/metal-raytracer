#include <metal_stdlib>
using namespace metal;

#include "../shaders/ray.metal"

#ifndef METAL_MATH
#define METAL_MATH

float3 getPoint(Ray ray, float t)
{
    return ray.origin + t * ray.dir;
}

bool checkTriangleIntersect(float3 v0, float3 v1, float3 v2, Ray ray, device float *tNear)
{
    // triangle is counter-clockwise aligned
    float3 ab = v1 - v0;
    float3 ac = v2 - v0;
    float3 n  = cross(ab, ac);

    // check if denominator is not 0
    float denominator = dot(ray.dir, n);
    if(denominator == 0.0) 
    {
        return false;
    }

    // check if t greater than 0
    float t = dot(v0 - ray.origin, n) / denominator;
    if(t < 0.0)
    {
        return false;
    }

    float3 p = getPoint(ray, t);
    
    float3 bc = v2 - v1;
    float3 ca = v0 - v2;
    float3 sign_dir = cross(ab, ac);
    bool sign_ab = dot(sign_dir, cross(ab, p - v0)) > 0;
    bool sign_bc = dot(sign_dir, cross(bc, p - v1)) > 0;
    bool sign_ca = dot(sign_dir, cross(ca, p - v2)) > 0;

    if(sign_ab && sign_bc && sign_ca) {
        *tNear = t;
        return true;
    }
    return false;
}


#endif // METAL_MATH