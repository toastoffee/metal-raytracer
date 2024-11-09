#include <metal_stdlib>
using namespace metal;

half4 sample_skybox(float3 dir,
                    texture2d< half, access::sample > skybox_front,
                    texture2d< half, access::sample > skybox_back,
                    texture2d< half, access::sample > skybox_left,
                    texture2d< half, access::sample > skybox_right,
                    texture2d< half, access::sample > skybox_top,
                    texture2d< half, access::sample > skybox_bottom)
{

    constexpr sampler s( address::clamp_to_edge, filter::linear );

    // forward
    if(dir.z > 0.0) 
    {
        float t = 0.5 / dir.z;
        float intersect_u = t * dir.x;
        float intersect_v = t * dir.y;

        if(intersect_u >= -0.5 && intersect_u <= 0.5
            && intersect_v >= -0.5 && intersect_v <= 0.5)
        {
            float u = (intersect_u + 0.5) / 1.0;
            float v = (intersect_v + 0.5) / 1.0;

            float2 texcoords = float2(u, 1.0 - v);
            half3 texel = skybox_front.sample( s, texcoords ).rgb;

            return half4( texel, 1.0 );
        }
    }

    // back
    if(dir.z <= 0.0) 
    {
        float t = -0.5 / dir.z;
        float intersect_u = t * dir.x;
        float intersect_v = t * dir.y;

        if(intersect_u >= -0.5 && intersect_u <= 0.5
            && intersect_v >= -0.5 && intersect_v <= 0.5)
        {
            float u = (intersect_u + 0.5) / 1.0;
            float v = (intersect_v + 0.5) / 1.0;

            float2 texcoords = float2(1.0 - u, 1.0 - v);
            half3 texel = skybox_back.sample( s, texcoords ).rgb;

            return half4( texel, 1.0 );
        }
    }

    // right
    if(dir.x > 0.0) 
    {
        float t = 0.5 / dir.x;
        float intersect_u = t * dir.z;
        float intersect_v = t * dir.y;

        if(intersect_u >= -0.5 && intersect_u <= 0.5
            && intersect_v >= -0.5 && intersect_v <= 0.5)
        {
            float u = (intersect_u + 0.5) / 1.0;
            float v = (intersect_v + 0.5) / 1.0;

            float2 texcoords = float2(1.0 - u, 1.0 - v);
            half3 texel = skybox_right.sample( s, texcoords ).rgb;

            return half4( texel, 1.0 );
        }
    }

    // left
    if(dir.x <= 0.0) 
    {
        float t = -0.5 / dir.x;
        float intersect_u = t * dir.z;
        float intersect_v = t * dir.y;

        if(intersect_u >= -0.5 && intersect_u <= 0.5
            && intersect_v >= -0.5 && intersect_v <= 0.5)
        {
            float u = (intersect_u + 0.5) / 1.0;
            float v = (intersect_v + 0.5) / 1.0;

            float2 texcoords = float2(u, 1.0 - v);
            half3 texel = skybox_left.sample( s, texcoords ).rgb;

            return half4( texel, 1.0 );
        }
    }

    // top
    if(dir.y > 0.0) 
    {
        float t = 0.5 / dir.y;
        float intersect_u = t * dir.x;
        float intersect_v = t * dir.z;

        if(intersect_u >= -0.5 && intersect_u <= 0.5
            && intersect_v >= -0.5 && intersect_v <= 0.5)
        {
            float u = (intersect_u + 0.5) / 1.0;
            float v = (intersect_v + 0.5) / 1.0;

            float2 texcoords = float2(u, v);
            half3 texel = skybox_top.sample( s, texcoords ).rgb;

            return half4( texel, 1.0 );
        }
    }

    // bottom
    if(dir.y <= 0.0) 
    {
        float t = -0.5 / dir.y;
        float intersect_u = t * dir.x;
        float intersect_v = t * dir.z;

        if(intersect_u >= -0.5 && intersect_u <= 0.5
            && intersect_v >= -0.5 && intersect_v <= 0.5)
        {
            float u = (intersect_u + 0.5) / 1.0;
            float v = (intersect_v + 0.5) / 1.0;

            float2 texcoords = float2(u, 1.0-v);
            half3 texel = skybox_bottom.sample( s, texcoords ).rgb;

            return half4( texel, 1.0 );
        }
    }

    return half4(1.0, 0.0, 0.0, 1.0);
}

struct CameraData
{
    float4x4 rotationMatrix;
    float4x4 translateMatrix;
};

kernel void computeMain(texture2d< half, access::write > tex            [[texture(0)]],
                        texture2d< half, access::sample > skybox_front  [[texture(1)]],
                        texture2d< half, access::sample > skybox_back   [[texture(2)]],
                        texture2d< half, access::sample > skybox_left   [[texture(3)]],
                        texture2d< half, access::sample > skybox_right  [[texture(4)]],
                        texture2d< half, access::sample > skybox_top    [[texture(5)]],
                        texture2d< half, access::sample > skybox_bottom [[texture(6)]],

                        device const CameraData& cameraData     [[buffer(0)]],
                        device const uint*       sample_count   [[buffer(1)]],

                        uint2 index [[thread_position_in_grid]],
                        uint2 gridSize [[threads_per_grid]])
{

    constexpr float fov = 90.0;
    constexpr float aspectRatio = 16.0 / 9.0;
    constexpr float viewportDist = 1.0;


    float viewportHeight = 2.0 * tan(fov * M_PI_H / 360.0) * viewportDist;
    float viewportWidth = viewportHeight * aspectRatio;


    float3 viewportForward = {0, 0, viewportDist};
    float3 viewportUp      = {0, viewportHeight, 0};
    float3 viewportRight   = {viewportWidth, 0, 0};


    float3 viewportLeftBtm = viewportForward - viewportUp / 2 - viewportRight / 2;
    
    float3 dir = viewportLeftBtm 
                + ((index.x + 0.5) / 1920.0) * viewportRight
                + ((index.y + 0.5) / 1080.0) * viewportUp;

    float4 dir4 = {dir.x, dir.y, dir.z, 1};
    dir4 = cameraData.rotationMatrix * dir4;

    dir = {dir4.x, dir4.y, dir4.z};

    dir = normalize(dir);

    tex.write(sample_skybox(dir, skybox_front, skybox_back, skybox_left, skybox_right, skybox_top, skybox_bottom), index, 0);
    // tex.write(half4(dir.x, dir.y, dir.z, 1.0), index, 0);
}
