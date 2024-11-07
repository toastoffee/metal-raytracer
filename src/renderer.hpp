/**
  ******************************************************************************
  * @file           : renderer.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/15
  ******************************************************************************
  */



#ifndef METAL_RENDERER_RENDERER_HPP
#define METAL_RENDERER_RENDERER_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class Renderer {
private:
    MTL::Device* _device;
    MTL::CommandQueue* _viewCommandQueue;
    MTL::RenderPipelineState* _viewPSO;
    MTL::ComputePipelineState* _computePSO;

    MTL::Buffer* _viewVertexDataBuffer;
    MTL::Buffer* _viewIndexBuffer;
    MTL::Texture* _texture;

    MTL::Buffer* _textureAnimBuffer;

    MTL::Texture* _skyboxFront;
    MTL::Texture* _skyboxBack;
    MTL::Texture* _skyboxLeft;
    MTL::Texture* _skyboxRight;
    MTL::Texture* _skyboxTop;
    MTL::Texture* _skyboxBottom;

    uint _animIdx;

public:
    explicit Renderer( MTL::Device* device );

    ~Renderer();

    void Draw(MTK::View* view);

    void BuildViewBuffers();

    void BuildShaders();
    void BuildTextures();

    void GenerateMandelbrotTexture(MTL::CommandBuffer* commandBuffer);
};


#endif //METAL_RENDERER_RENDERER_HPP
