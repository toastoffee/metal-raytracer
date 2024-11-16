#include <metal_stdlib>
using namespace metal;

#include "../shaders/random.metal"
#include "../shaders/skybox.metal"
#include "../shaders/color.metal"
#include "../shaders/camera.metal"
#include "../shaders/math.metal"

struct HitPayload 
{
    float3 p;
    float t;
    float3 normal;
    bool hitFront;    
};

struct Mesh
{
    device const float3* vertices;
    device const uint3*  indices;
    device const uint*   face_count;
};

half4 rayColor( Ray ray,
                Cubemap cubemap, 
                Mesh mesh,
                int depth,
                float seed)
{
    // if exceeded the ray bounce limit, then we assume that no more lights
    if(depth <= 0) {
        return half4{1.f, 0.f, 0.f, 1.f};
    }

    // if hit object, then scatter and raycast again

    float tNear = 10000000.f;
    float t = 0.0f;

    float3 v0, v1, v2, p, normal;

    bool hit_anything = false;

    half4 attenuation = {0.5, 0.5, 0.5, 1.0};

    half4 final_color = {1.0, 1.0, 1.0, 1.0};

    for(uint sample = 0; sample < depth; sample++)
    {
        hit_anything = false;

        // check if hit
        for(uint i = 0; i < *mesh.face_count; i++) 
        {
            uint3 index = mesh.indices[i];
            float3 v0 = mesh.vertices[index.x];
            float3 v1 = mesh.vertices[index.y];        
            float3 v2 = mesh.vertices[index.z];

            if(checkTriangleIntersect(v0, v1, v2, ray, &t)) {
                if(t >= 0.01f) {
                    hit_anything = true;

                    // update relected rays
                    if(t < tNear) {
                        tNear = t;
                        p = getPoint(ray, tNear);
                        float3 outward_normal = normalize(cross(v1 - v0, v2 - v0));
                        // set correct normal direction
                        bool hit_front = dot(ray.dir, outward_normal) < 0.0;
                        normal = hit_front ? outward_normal : -outward_normal;
                    }
                }
            }
        }

        if(hit_anything) {
            // scatter
            float3 scattered = normalize(ray.dir - 2.0f * dot(ray.dir, normal) * normal);  
            
            float3 fuzz_sphere = 0.1 * randUnitFloat3(seed);
            scattered = normalize(scattered + fuzz_sphere);

            // modify rays
            ray.origin = p;
            ray.dir = scattered;

            final_color *= attenuation;
        }
        else {
            final_color *= sample_skybox(ray.dir, cubemap);
            break;
        }
    }

    return final_color;

}


kernel void computeMain(texture2d< half, access::read_write > tex       [[texture(0)]],
                        texture2d< half, access::sample > skybox_front  [[texture(1)]],
                        texture2d< half, access::sample > skybox_back   [[texture(2)]],
                        texture2d< half, access::sample > skybox_left   [[texture(3)]],
                        texture2d< half, access::sample > skybox_right  [[texture(4)]],
                        texture2d< half, access::sample > skybox_top    [[texture(5)]],
                        texture2d< half, access::sample > skybox_bottom [[texture(6)]],

                        device const CameraData& cameraData         [[buffer(0)]],
                        device       uint*       sample_count       [[buffer(1)]],
                        device const float3*     mesh_vertices      [[buffer(2)]],
                        device const uint3*      mesh_indices       [[buffer(3)]],
                        device       uint*       mesh_indices_count [[buffer(4)]],

                        uint2 index [[thread_position_in_grid]],
                        uint2 gridSize [[threads_per_grid]])
{
    uint sample_interval = 1;
    uint quotient = *sample_count / sample_interval;
    uint remainder = *sample_count % sample_interval;
    uint align_index = index.x + index.y * gridSize.x;

    if(align_index % sample_interval != remainder) {
        return;
    }

    constexpr float fov = 90.0;
    float seed = (align_index + sin((float)*sample_count) * (gridSize.x * gridSize.y)) * 0.1;
    Cubemap cubemap = {skybox_front, skybox_back, skybox_left, skybox_right, skybox_top, skybox_bottom};

    Ray ray = getRandRay(index, fov, gridSize, cameraData, seed);
    float3 dir = ray.dir;

    Mesh mesh = {mesh_vertices, mesh_indices, mesh_indices_count};

    // multisample -> mix color
    half4 current_color = rayColor(ray, cubemap, mesh, 10, seed);
 
    half4 former_color = tex.read(index);

    float mix_ratio = quotient / (quotient + 1.0);

    // half4 final_color = mix_ratio * former_color + (1.0 - mix_ratio) * current_color;
    half4 final_color = mixColor(former_color, current_color, mix_ratio, 1.0 - mix_ratio);

    tex.write(final_color, index, 0);
    // tex.write(half4(randDir.x, randDir.y, randDir.z, 1.0), index, 0);
}
