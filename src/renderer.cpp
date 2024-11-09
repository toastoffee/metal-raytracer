/**
  ******************************************************************************
  * @file           : renderer.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/15
  ******************************************************************************
  */


#include <simd/simd.h>
#include <stbi/stb_image.h>
#include <iostream>

#include "renderer.hpp"
#include "shader_tool.hpp"
#include "shader_types.hpp"


static constexpr uint32_t kTextureWidth = 1920;
static constexpr uint32_t kTextureHeight = 1080;

Renderer::Renderer(MTL::Device *device)
: _device( device->retain() ),
  _animIdx(0)
{
    _viewCommandQueue = _device->newCommandQueue();

    BuildViewBuffers();
    BuildShaders();
    BuildTextures();
}

Renderer::~Renderer() {
    _viewIndexBuffer->release();
    _viewVertexDataBuffer->release();
    _viewPSO->release();
    _viewCommandQueue->release();
    _device->release();
}

void Renderer::Draw(MTK::View *view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* cmd = _viewCommandQueue->commandBuffer();


    GenerateMandelbrotTexture(cmd);

    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    enc->setRenderPipelineState(_viewPSO);

    enc->setVertexBuffer(_viewVertexDataBuffer, 0, 0);
    enc->setFragmentTexture( _texture, /* index */ 0 );

    enc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);
    enc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                               6 , MTL::IndexType::IndexTypeUInt16,
                               _viewIndexBuffer, 0);

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}

void Renderer::BuildViewBuffers() {

    shader_types::VertexData vertices[] = {
            {{-1, -1, 0}, {0, 0}},
            {{+1, -1, 0}, {1, 0}},
            {{-1, +1, 0}, {0, 1}},
            {{+1, +1, 0}, {1, 1}}
    };

    uint16_t indices[] = {
            0, 1, 2,
            2, 1, 3
    };

    const size_t vertexDataSize = sizeof(vertices);
    const size_t indexDataSize = sizeof(indices);

    MTL::Buffer* vertexBuffer = _device->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* indexBuffer = _device->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

    _viewVertexDataBuffer = vertexBuffer;
    _viewIndexBuffer = indexBuffer;

    memcpy( _viewVertexDataBuffer->contents(), vertices, vertexDataSize );
    memcpy( _viewIndexBuffer->contents(), indices, indexDataSize );

    _viewVertexDataBuffer->didModifyRange(NS::Range::Make(0, _viewVertexDataBuffer->length()));
    _viewIndexBuffer->didModifyRange(NS::Range::Make(0, _viewIndexBuffer->length()));

    _textureAnimBuffer = _device->newBuffer(sizeof(uint), MTL::ResourceStorageModeManaged);
}

void Renderer::BuildCameraBuffer() {
//    shader_types::CameraData cameraData{
//
//    }
}

void Renderer::BuildShaders() {
    _viewPSO = ShaderTool::loadShader("../shaders/view.metal", _device);
    _computePSO = ShaderTool::loadComputeShader("../shaders/raytrace.metal", _device);
}


void Renderer::BuildTextures() {
    _texture =  ShaderTool::createTexture(kTextureWidth, kTextureHeight, _device);

    // build skybox textures
    _skyboxFront = ShaderTool::loadTexture("../static/skybox/front.jpg", _device);
    _skyboxBack = ShaderTool::loadTexture("../static/skybox/back.jpg", _device);
    _skyboxLeft = ShaderTool::loadTexture("../static/skybox/left.jpg", _device);
    _skyboxRight = ShaderTool::loadTexture("../static/skybox/right.jpg", _device);
    _skyboxTop = ShaderTool::loadTexture("../static/skybox/top.jpg", _device);
    _skyboxBottom = ShaderTool::loadTexture("../static/skybox/bottom.jpg", _device);
}

void Renderer::GenerateMandelbrotTexture(MTL::CommandBuffer *commandBuffer) {

    assert(commandBuffer);

    uint* ptr = reinterpret_cast<uint*>(_textureAnimBuffer->contents());
    *ptr = (_animIdx++) % 5000;
    _textureAnimBuffer->didModifyRange(NS::Range::Make(0, sizeof(uint)));

    MTL::ComputeCommandEncoder* computeCmdEnc = commandBuffer->computeCommandEncoder();

    computeCmdEnc->setComputePipelineState(_computePSO);
    computeCmdEnc->setTexture(_texture, 0);
    computeCmdEnc->setTexture(_skyboxFront, 1);
    computeCmdEnc->setTexture(_skyboxBack, 2);
    computeCmdEnc->setTexture(_skyboxLeft, 3);
    computeCmdEnc->setTexture(_skyboxRight, 4);
    computeCmdEnc->setTexture(_skyboxTop, 5);
    computeCmdEnc->setTexture(_skyboxBottom, 6);
    computeCmdEnc->setBuffer(_textureAnimBuffer, 0, 0);

    MTL::Size gridSize = MTL::Size(kTextureWidth, kTextureHeight, 1);

    NS::UInteger threadSize = _computePSO->maxTotalThreadsPerThreadgroup();
    MTL::Size threadGroupSize(threadSize, 1, 1);

    computeCmdEnc->dispatchThreads(gridSize, threadGroupSize);

    computeCmdEnc->endEncoding();
}


