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
    _viewPSO = ShaderTool::loadShader("../shaders/view.metal", _device);
}

void Renderer::BuildViewBuffers() {

}
