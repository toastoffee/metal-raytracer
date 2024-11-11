#include <metal_stdlib>
using namespace metal;

#ifndef METAL_RANDOM
#define METAL_RANDOM

constant float deg2rad = M_PI_H / 180.0;

float rand(float seed) {
    
    float2 p2 = {seed * 1.231, seed * 3.613};
    
    float3 p3 = {fract(p2.x * 0.1031), fract(p2.y * 0.1031), fract(p2.x * 0.1031)};
    float3 p4 = {p3.y + 33.33, p3.z + 33.33, p3.x + 33.33};
    float dotProduct = dot(p3, p4);

    p3 += dotProduct;

    return fract((p3.x + p3.y) * p3.z);

    // return fract(sin(seed) * 43758.5453);
}

// Generate a random float in the range [min, max]
float randRange(float seed, float min, float max) {
    return min + rand(seed) * (max - min);
}

// Generate a random float3 in unit sphere
float3 randUnitFloat3(float seed) {
    float theta = randRange(seed, 0.0, 180.0) * deg2rad;
    float phi = randRange(seed * 2, 0.0, 360.0) * deg2rad;

    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);

    return float3{x, y, z};
}

#endif // METAL_RANDOM