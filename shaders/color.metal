#include <metal_stdlib>
using namespace metal;


half4 mixColor(half4 colorA, half4 colorB, float ratioA, float ratioB)
{
    return colorA * ratioA + colorB * ratioB;
}