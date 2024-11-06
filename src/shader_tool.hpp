/**
  ******************************************************************************
  * @file           : ShaderTool.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/16
  ******************************************************************************
  */



#ifndef METAL_RENDERER_SHADER_TOOL_HPP
#define METAL_RENDERER_SHADER_TOOL_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class ShaderTool {
private:
    static std::string readSource(const char *filePath);
    static MTL::Library* createLibrary(const char* shaderFilePath, MTL::Device* device);

public:
    static MTL::RenderPipelineState* loadShader(const char* shaderFilePath, MTL::Device* device);
    static MTL::ComputePipelineState* loadComputeShader(const char* shaderFilePath, MTL::Device* device);
    static MTL::Texture* loadTexture(const char *textureFilePath, MTL::Device* device, int& width, int &height);
};


#endif //METAL_RENDERER_SHADER_TOOL_HPP
