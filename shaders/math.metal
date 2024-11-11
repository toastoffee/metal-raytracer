#include <metal_stdlib>
using namespace metal;

#include "../shaders/ray.metal"

#ifndef METAL_MATH
#define METAL_MATH

bool checkTriangleIntersect(float3  v0,
                            float3  v1,
                            float3  v2,
                            Ray     ray,
                            float   tNear)
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

}


#endif // METAL_MATH