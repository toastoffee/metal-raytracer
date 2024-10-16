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
#include "file_helper.hpp"

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

    std::string viewShaderSrc = FileHelper::readSource("../shaders/view.metal");
    

}
