#include <metal_stdlib>
using namespace metal;

#include "../shaders/random.metal"

struct CameraData
{
    float4x4 rotationMatrix;
    float4x4 translateMatrix;
};

struct Ray
{
    float3 origin;
    float3 dir;
};

Ray getRay(uint2 index,
            float fov,
            uint2 resolution,
            CameraData cameraData)
{
    float aspectRatio = resolution.x / resolution.y;
    
    // compute direction
    constexpr float viewportDist = 1.0;

    float viewportHeight = 2.0 * tan(fov * M_PI_H / 360.0) * viewportDist;
    float viewportWidth = viewportHeight * aspectRatio;

    float3 viewportForward = {0, 0, viewportDist};
    float3 viewportUp      = {0, viewportHeight, 0};
    float3 viewportRight   = {viewportWidth, 0, 0};

    float3 viewportLeftBtm = viewportForward - viewportUp / 2 - viewportRight / 2;

    float3 dir = viewportLeftBtm 
            + ((index.x + 0.5) / resolution.x) * viewportRight
            + ((index.y + 0.5) / resolution.y) * viewportUp;

    float4 dir4 = {dir.x, dir.y, dir.z, 1};
    dir4 = cameraData.rotationMatrix * dir4;
    dir = {dir4.x, dir4.y, dir4.z};
    dir = normalize(dir);


    // compute origin
    float4 origin4 = {0.0, 0.0, 0.0, 1.0};
    origin4 = cameraData.translateMatrix * origin4;
    float3 origin = {origin4.x, origin4.y, origin4.z};

    return Ray{origin, dir};
}

Ray getRandRay(uint2 index,
            float fov,
            uint2 resolution,
            CameraData cameraData,
            float seed)
{
    float aspectRatio = resolution.x / resolution.y;
    
    // compute direction
    constexpr float viewportDist = 1.0;

    float viewportHeight = 2.0 * tan(fov * M_PI_H / 360.0) * viewportDist;
    float viewportWidth = viewportHeight * aspectRatio;

    float3 viewportForward = {0, 0, viewportDist};
    float3 viewportUp      = {0, viewportHeight, 0};
    float3 viewportRight   = {viewportWidth, 0, 0};

    float3 viewportLeftBtm = viewportForward - viewportUp / 2 - viewportRight / 2;

    float3 dir = viewportLeftBtm 
            + ((index.x + rand(seed)) / resolution.x) * viewportRight
            + ((index.y + rand(seed * 2)) / resolution.y) * viewportUp;

    float4 dir4 = {dir.x, dir.y, dir.z, 1};
    dir4 = cameraData.rotationMatrix * dir4;
    dir = {dir4.x, dir4.y, dir4.z};
    dir = normalize(dir);


    // compute origin
    float4 origin4 = {0.0, 0.0, 0.0, 1.0};
    origin4 = cameraData.translateMatrix * origin4;
    float3 origin = {origin4.x, origin4.y, origin4.z};

    return Ray{origin, dir};
}