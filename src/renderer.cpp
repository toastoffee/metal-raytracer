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
#include "matrix_tool.hpp"
#include "shader_types.hpp"
#include "mesh_tool.hpp"


static constexpr uint32_t kTextureWidth = 1920;
static constexpr uint32_t kTextureHeight = 1080;

Renderer::Renderer(MTL::Device *device)
: _device( device->retain() ),
  _sampleCount(0)
{
    _viewCommandQueue = _device->newCommandQueue();

    BuildBuffers();
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


    GenerateRaytraceTexture(cmd);

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

    _viewVertexDataBuffer = ShaderTool::createBuffer(vertices, vertexDataSize, _device);
    _viewIndexBuffer = ShaderTool::createBuffer(indices, indexDataSize, _device);
}

void Renderer::BuildCameraBuffer() {
    shader_types::CameraData cameraData{
        MatrixTool::makeYRotate(2.0 / 2.0), MatrixTool::makeIdentity()
    };

    const size_t cameraDataSize = sizeof(cameraData);
    _cameraDataBuffer = ShaderTool::createBuffer(&cameraData, cameraDataSize, _device);
}

void Renderer::BuildBuffers() {

    // 1. view vertex data buffer
    shader_types::VertexData vertices[] = {
            {{-1, -1, 0}, {0, 0}},
            {{+1, -1, 0}, {1, 0}},
            {{-1, +1, 0}, {0, 1}},
            {{+1, +1, 0}, {1, 1}}
    };
    const size_t vertexDataSize = sizeof(vertices);
    _viewVertexDataBuffer = ShaderTool::createBuffer(vertices, vertexDataSize, _device);

    // 2. view indices data buffer
    uint16_t indices[] = {
            0, 1, 2,
            2, 1, 3
    };
    const size_t indexDataSize = sizeof(indices);
    _viewIndexBuffer = ShaderTool::createBuffer(indices, indexDataSize, _device);

    // 3. camera data buffer
    shader_types::CameraData cameraData{
            MatrixTool::makeYRotate(0), MatrixTool::makeIdentity()
    };
    const size_t cameraDataSize = sizeof(cameraData);
    _cameraDataBuffer = ShaderTool::createBuffer(&cameraData, cameraDataSize, _device);

    // 4. sample count buffer
    _sampleCountBuffer = ShaderTool::createEmptyBuffer(sizeof(uint), _device);

    // 5. mesh vertices/indices/indices count buffers
    auto mesh = MeshTool::loadMesh("../static/bunny.obj");

    const size_t meshVerticesSize = mesh.vertices.size() * sizeof(simd::float3);
    _meshVerticesBuffer = ShaderTool::createBuffer(mesh.vertices.data(), meshVerticesSize, _device);


}

void Renderer::BuildShaders() {
    _viewPSO    = ShaderTool::loadShader("../shaders/view.metal", _device);
    _computePSO = ShaderTool::loadComputeShader("../shaders/raytrace.metal", _device);
}


void Renderer::BuildTextures() {
    _texture =  ShaderTool::createTexture(kTextureWidth, kTextureHeight, _device);

    // build skybox textures
    _skyboxFront  = ShaderTool::loadTexture("../static/skybox/front.jpg", _device);
    _skyboxBack   = ShaderTool::loadTexture("../static/skybox/back.jpg", _device);
    _skyboxLeft   = ShaderTool::loadTexture("../static/skybox/left.jpg", _device);
    _skyboxRight  = ShaderTool::loadTexture("../static/skybox/right.jpg", _device);
    _skyboxTop    = ShaderTool::loadTexture("../static/skybox/top.jpg", _device);
    _skyboxBottom = ShaderTool::loadTexture("../static/skybox/bottom.jpg", _device);
}

void Renderer::GenerateRaytraceTexture(MTL::CommandBuffer *commandBuffer) {

    assert(commandBuffer);

    uint* ptr = reinterpret_cast<uint*>(_sampleCountBuffer->contents());
    *ptr = _sampleCount++;
    _sampleCountBuffer->didModifyRange(NS::Range::Make(0, sizeof(uint)));

    std::cout << *ptr << std::endl;

    MTL::ComputeCommandEncoder* computeCmdEnc = commandBuffer->computeCommandEncoder();

    computeCmdEnc->setComputePipelineState(_computePSO);
    computeCmdEnc->setTexture(_texture, 0);
    computeCmdEnc->setTexture(_skyboxFront, 1);
    computeCmdEnc->setTexture(_skyboxBack, 2);
    computeCmdEnc->setTexture(_skyboxLeft, 3);
    computeCmdEnc->setTexture(_skyboxRight, 4);
    computeCmdEnc->setTexture(_skyboxTop, 5);
    computeCmdEnc->setTexture(_skyboxBottom, 6);


    computeCmdEnc->setBuffer(_cameraDataBuffer, 0, 0);
    computeCmdEnc->setBuffer(_sampleCountBuffer, 0, 1);

    MTL::Size gridSize = MTL::Size(kTextureWidth, kTextureHeight, 1);

    NS::UInteger threadSize = _computePSO->maxTotalThreadsPerThreadgroup();
    MTL::Size threadGroupSize(threadSize, 1, 1);

    computeCmdEnc->dispatchThreads(gridSize, threadGroupSize);

    computeCmdEnc->endEncoding();
}



