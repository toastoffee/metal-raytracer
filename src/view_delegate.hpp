/**
  ******************************************************************************
  * @file           : view_delegate.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/14
  ******************************************************************************
  */



#ifndef METAL_RENDERER_VIEW_DELEGATE_HPP
#define METAL_RENDERER_VIEW_DELEGATE_HPP

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class ViewDelegate : public MTK::ViewDelegate {
public:
    ViewDelegate(MTL::Device* device);

    ~ViewDelegate() override;

    void drawInMTKView(MTK::View* pView) override;
};


#endif //METAL_RENDERER_VIEW_DELEGATE_HPP
