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

    return half4(0.0, 0.0, 0.0, 1.0);
}