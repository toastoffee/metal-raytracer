/**
  ******************************************************************************
  * @file           : matrix_tool.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/9
  ******************************************************************************
  */



#ifndef METAL_RENDERER_MATRIX_TOOL_HPP
#define METAL_RENDERER_MATRIX_TOOL_HPP

#include <simd/simd.h>

class MatrixTool {
    static simd::float4x4 makeIdentity();
    static simd::float4x4 makeXRotate(float radians);
    static simd::float4x4 makeYRotate(float radians);
    static simd::float4x4 makeZRotate(float radians);
    static simd::float4x4 makeTranslate(const simd::float3 &v);
    static simd::float4x4 makeScale(const simd::float3 &v);
};


#endif //METAL_RENDERER_MATRIX_TOOL_HPP
