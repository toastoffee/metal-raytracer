#include <metal_stdlib>
using namespace metal;

constant float deg2rad = M_PI_H / 180.0;

// Generate a random float in the range [0.0f, 1.0f]
float rand(float seed) {
    // fake-pseudo algorithm
    return fract(sin(seed) * 43758.5453);
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