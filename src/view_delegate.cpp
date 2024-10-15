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

ViewDelegate::ViewDelegate(MTL::Device *device) :
        MTK::ViewDelegate(),
        { }

ViewDelegate::~ViewDelegate() {

}

void ViewDelegate::drawInMTKView(MTK::View *pView) {
    ViewDelegate::drawInMTKView(pView);
}


