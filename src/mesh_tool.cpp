/**
  ******************************************************************************
  * @file           : mesh_tool.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/11/13
  ******************************************************************************
  */



#include "mesh_tool.hpp"

void MeshTool::processNode(aiNode *node, const aiScene *scene, const simd::float3 pos) {

    // process all meshes and load triangles

}

Mesh MeshTool::convertMesh(aiMesh *mesh, const aiScene *scene) {
    using simd::float3;
    using simd::uint3;

    Mesh ret;

    // handle vertices
    for (int i = 0; i < mesh->mNumVertices; ++i) {
        float3 vertex = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        ret.vertices.push_back(vertex);
    }

    // handle indices
    for (int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];

        for (int j = 0; j < face.mNumIndices; j += 3) {
            uint3 index = {face.mIndices[j], face.mIndices[j+1], face.mIndices[j+2]};
            ret.indices.push_back(index);
        }
    }

    return ret;
}

