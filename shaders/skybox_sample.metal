#include <metal_stdlib>
using namespace metal;

half4 sample_skybox(float3 dir)
{
    return half4(dir.x, dir.y, dir.z, 1.0);
}