/**
  ******************************************************************************
  * @file           : mesh.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/14
  ******************************************************************************
  */



#ifndef METAL_RENDERER_MESH_HPP
#define METAL_RENDERER_MESH_HPP

#include <vector>
#include <simd/simd.h>

class Mesh {
public:
    std::vector<simd::float3> vertices;
    std::vector<simd::uint3> indices;

    void mergeMesh(Mesh &mesh);
};


#endif //METAL_RENDERER_MESH_HPP
