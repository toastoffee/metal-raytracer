/**
  ******************************************************************************
  * @file           : view_delegate.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/14
  ******************************************************************************
  */



#include "view_delegate.hpp"
#include "renderer.hpp"

ViewDelegate::ViewDelegate(MTL::Device *device) :
        MTK::ViewDelegate(),
        _renderer( new Renderer(device) ) { }

ViewDelegate::~ViewDelegate() {
    delete _renderer;
}

void ViewDelegate::drawInMTKView(MTK::View *pView) {
    _renderer->Draw(pView);
}


