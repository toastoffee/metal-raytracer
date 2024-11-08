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

    constexpr sampler s( address::repeat, filter::linear );

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

    //         float2 texcoords = float2(1.0, 0.5);
    //         half3 texel = skybox_bottom.sample( s, texcoords ).rgb;

    //         return half4( texel, 1.0 );
    //     }
    // }

    return half4(dir.x, dir.y, dir.z, 1.0);
}