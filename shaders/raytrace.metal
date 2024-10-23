#include <metal_stdlib>
using namespace metal;

kernel void computeMain(texture2d< half, access::write > tex [[texture(0)]],
                            uint2 index [[thread_position_in_grid]],
                            uint2 gridSize [[threads_per_grid]],
                            device const uint* frame [[buffer(0)]])
{

    constexpr float fov = 90.0;
    constexpr float aspectRatio = 16.0 / 9.0;
    constexpr float viewportDist = 1.0;


    float viewportHeight = 2.0 * tan(fov * M_PI_H / 360.0) * viewportDist;
    float viewportWidth = viewportHeight * aspectRatio;


    float3 origin = {0, 0, 0};
    float3 viewportForward = {0, 0, viewportDist};
    float3 viewportUp      = {0, viewportHeight, 0};
    float3 viewportRight   = {viewportWidth, 0, 0};
    float3 viewportLeftBtm = origin + viewportForward - viewportUp / 2 - viewportRight / 2;
    
    float3 dir = viewportLeftBtm 
                + ((index.x + 0.5) / 1920.0) * viewportRight
                + ((index.y + 0.5) / 1080.0) * viewportUp;

    dir = normalize(dir);

    tex.write(half4(dir.x, dir.y, dir.z, 1.0), index, 0);
    
}