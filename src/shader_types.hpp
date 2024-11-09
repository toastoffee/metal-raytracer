/**
  ******************************************************************************
  * @file           : shader_types.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/21
  ******************************************************************************
  */



#ifndef METAL_RENDERER_SHADER_TYPES_HPP
#define METAL_RENDERER_SHADER_TYPES_HPP

#include <simd/simd.h>

namespace shader_types
{
    struct VertexData
    {
        simd::float3 position;
        simd::float2 texcoord;
    };

    struct CameraData
    {
        simd::float4x4 rotationMatrix;
        simd::float3 position;
    };
}

#endif //METAL_RENDERER_SHADER_TYPES_HPP
