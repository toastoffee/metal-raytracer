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

#include "renderer.hpp"
#include "shader_tool.hpp"
#include "shader_types.hpp"

Renderer::Renderer(MTL::Device *device)
: _device( device->retain() )
{
    _viewCommandQueue = _device->newCommandQueue();

    BuildViewBuffers();
    BuildViewShaders();
}

Renderer::~Renderer() {

}

void Renderer::Draw(MTK::View *view) {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* cmd = _viewCommandQueue->commandBuffer();

    MTL::RenderPassDescriptor* rpd = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* enc = cmd->renderCommandEncoder(rpd);

    enc->setRenderPipelineState(_viewPSO);
    enc->setVertexBuffer(_viewVertexDataBuffer, 0, 0);
    enc->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);
    enc->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                               6 , MTL::IndexType::IndexTypeUInt16,
                               _viewIndexBuffer, 0);

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}

void Renderer::BuildViewShaders() {
    _viewPSO = ShaderTool::loadShader("../shaders/view.metal", _device);
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
}
