#include <metal_stdlib>
using namespace metal;

#include "../shaders/random.metal"
#include "../shaders/skybox.metal"
#include "../shaders/color.metal"
#include "../shaders/camera.metal"



half3 rayColor( float3 dir, 
                int depth,
                texture2d< half, access::sample > skybox_front,
                texture2d< half, access::sample > skybox_back,
                texture2d< half, access::sample > skybox_left,
                texture2d< half, access::sample > skybox_right,
                texture2d< half, access::sample > skybox_top,
                texture2d< half, access::sample > skybox_bottom)
{
    // if exceeded the ray bounce limit, then we assume that no more lights
    if(depth <= 0) {
        return half3{0.f, 0.f, 0.f};
    }

    // if hit object, then scatter and raycast again
    
}


kernel void computeMain(texture2d< half, access::read_write > tex       [[texture(0)]],
                        texture2d< half, access::sample > skybox_front  [[texture(1)]],
                        texture2d< half, access::sample > skybox_back   [[texture(2)]],
                        texture2d< half, access::sample > skybox_left   [[texture(3)]],
                        texture2d< half, access::sample > skybox_right  [[texture(4)]],
                        texture2d< half, access::sample > skybox_top    [[texture(5)]],
                        texture2d< half, access::sample > skybox_bottom [[texture(6)]],

                        device const CameraData& cameraData     [[buffer(0)]],
                        device       uint*       sample_count   [[buffer(1)]],

                        uint2 index [[thread_position_in_grid]],
                        uint2 gridSize [[threads_per_grid]])
{

    constexpr float fov = 90.0;

    float seed = (index.x + index.y * gridSize.x + sin((float)*sample_count) * (gridSize.x * gridSize.y)) * 0.1;

    Ray ray = getRandRay(index, fov, gridSize, cameraData, seed);
    float3 dir = ray.dir;

    // multisample -> mix color
    half4 current_color = sample_skybox(dir, skybox_front, skybox_back, skybox_left, skybox_right, skybox_top, skybox_bottom);
 
    half4 former_color = tex.read(index);

    float mix_ratio = *sample_count / (*sample_count + 1.0);

    // half4 final_color = mix_ratio * former_color + (1.0 - mix_ratio) * current_color;
    half4 final_color = mixColor(former_color, current_color, mix_ratio, 1.0 - mix_ratio);

    tex.write(final_color, index, 0);
    // tex.write(half4(randDir.x, randDir.y, randDir.z, 1.0), index, 0);
}
