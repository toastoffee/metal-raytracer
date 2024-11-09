/**
  ******************************************************************************
  * @file           : matrix_tool.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/9
  ******************************************************************************
  */



#include "matrix_tool.hpp"

simd::float4x4 MatrixTool::makeIdentity() {
    using simd::float4;
    return simd_matrix_from_rows((float4){ 1.f, 0.f, 0.f, 0.f },
                                 (float4){ 0.f, 1.f, 0.f, 0.f },
                                 (float4){ 0.f, 0.f, 1.f, 0.f },
                                 (float4){ 0.f, 0.f, 0.f, 1.f });
}

simd::float4x4 MatrixTool::makeXRotate(float radians) {
    using simd::float4;
    return simd_matrix_from_rows((float4){ 1.0f, 0.0f, 0.0f, 0.0f },
                                 (float4){ 0.0f, cosf( radians ), sinf( radians ), 0.0f },
                                 (float4){ 0.0f, -sinf( radians ), cosf( radians ), 0.0f },
                                 (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd::float4x4 MatrixTool::makeYRotate(float radians) {
    using simd::float4;
    return simd_matrix_from_rows((float4){ cosf( radians ), 0.0f, sinf( radians ), 0.0f },
                                 (float4){ 0.0f, 1.0f, 0.0f, 0.0f },
                                 (float4){ -sinf( radians ), 0.0f, cosf( radians ), 0.0f },
                                 (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd::float4x4 MatrixTool::makeZRotate(float radians) {
    using simd::float4;
    return simd_matrix_from_rows((float4){ cosf( radians ), sinf( radians ), 0.0f, 0.0f },
                                 (float4){ -sinf( radians ), cosf( radians ), 0.0f, 0.0f },
                                 (float4){ 0.0f, 0.0f, 1.0f, 0.0f },
                                 (float4){ 0.0f, 0.0f, 0.0f, 1.0f });
}

simd::float4x4 MatrixTool::makeTranslate(const simd::float3 &v) {
    using simd::float4;
    const float4 col0 = { 1.0f, 0.0f, 0.0f, 0.0f };
    const float4 col1 = { 0.0f, 1.0f, 0.0f, 0.0f };
    const float4 col2 = { 0.0f, 0.0f, 1.0f, 0.0f };
    const float4 col3 = { v.x, v.y, v.z, 1.0f };
    return simd_matrix( col0, col1, col2, col3 );
}

simd::float4x4 MatrixTool::makeScale(const simd::float3 &v) {
    using simd::float4;
    return simd_matrix((float4){ v.x, 0, 0, 0 },
                       (float4){ 0, v.y, 0, 0 },
                       (float4){ 0, 0, v.z, 0 },
                       (float4){ 0, 0, 0, 1.0 });
}


