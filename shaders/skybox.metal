#include <metal_stdlib>
using namespace metal;

struct Cubemap
{
    texture2d< half, access::sample > front;
    texture2d< half, access::sample > back;
    texture2d< half, access::sample > left;
    texture2d< half, access::sample > right;
    texture2d< half, access::sample > top;
    texture2d< half, access::sample > bottom;
};

half4 sample_skybox(float3 dir,
                    Cubemap cubemap)
{

    constexpr sampler s( address::clamp_to_edge, filter::linear );

    // forward
    // if(dir.z > 0.0) 
    // {
    //     float t = 0.5 / dir.z;
    //     float intersect_u = t * dir.x;
    //     float intersect_v = t * dir.y;

    //     if(intersect_u >= -0.5 && intersect_u <= 0.5
    //         && intersect_v >= -0.5 && intersect_v <= 0.5)
    //     {
    //         float u = (intersect_u + 0.5) / 1.0;
    //         float v = (intersect_v + 0.5) / 1.0;

    //         float2 texcoords = float2(u, 1.0 - v);
    //         half3 texel = cubemap.front.sample( s, texcoords ).rgb;

    //         return half4( texel, 1.0 );
    //     }
    // }

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
            half3 texel = cubemap.back.sample( s, texcoords ).rgb;

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
            half3 texel = cubemap.right.sample( s, texcoords ).rgb;

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
            half3 texel = cubemap.left.sample( s, texcoords ).rgb;

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
            half3 texel = cubemap.top.sample( s, texcoords ).rgb;

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
            half3 texel = cubemap.bottom.sample( s, texcoords ).rgb;

            return half4( texel, 1.0 );
        }
    }

    return half4(0.0, 0.0, 0.0, 1.0);
}