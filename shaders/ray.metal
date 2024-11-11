#include <metal_stdlib>
using namespace metal;

#ifndef METAL_RAY
#define METAL_RAY

struct Ray
{
    float3 origin;
    float3 dir;
};


#endif // METAL_RAY