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

Renderer::Renderer(MTL::Device *device)
: _device( device->retain() )
{
    _viewCommandQueue = _device->newCommandQueue();
}

Renderer::~Renderer() {

}

void Renderer::Draw(MTK::View *view) {

}

void Renderer::BuildViewShaders() {
    using NS::StringEncoding::UTF8StringEncoding;

    MTL::Library* library = ShaderTool::createLibrary("../shaders/view.metal", _device);

    MTL::Function* vertexFn = library->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::Function* fragFn = library->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(vertexFn);
    desc->setFragmentFunction(fragFn);
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatRGBA8Unorm_sRGB);


    _viewPSO = _device->newRenderPipelineState(desc, )

}
