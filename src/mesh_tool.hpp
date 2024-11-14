/**
  ******************************************************************************
  * @file           : mesh_tool.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/13
  ******************************************************************************
  */



#ifndef METAL_RENDERER_MESH_TOOL_HPP
#define METAL_RENDERER_MESH_TOOL_HPP

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include <simd/simd.h>

#include <mesh.hpp>

class MeshTool {
private:
    // process node recursively
    static void processNode(aiNode *node, const aiScene *scene, const simd::float3 pos);

    // transfer aiMesh to mesh
    static Mesh convertMesh(aiMesh *mesh, const aiScene *scene);

public:

};


#endif //METAL_RENDERER_MESH_TOOL_HPP
