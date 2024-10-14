/**
  ******************************************************************************
  * @file           : app_delegate.hpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/14
  ******************************************************************************
  */



#ifndef METAL_RENDERER_APP_DELEGATE_HPP
#define METAL_RENDERER_APP_DELEGATE_HPP

#include <Metal/Metal.hpp>
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

#include "view_delegate.hpp"

class  AppDelegate : public NS::ApplicationDelegate {
public:
    ~AppDelegate() override;

    NS::Menu* createMenuBar();

    void applicationWillFinishLaunching(NS::Notification *pNotification) override;

    void applicationDidFinishLaunching(NS::Notification *pNotification) override;

    bool applicationShouldTerminateAfterLastWindowClosed(NS::Application *pSender) override;

private:
    NS::Window* _window;
    MTK::View* _mtkView;
    MTL::Device* _device;
    ViewDelegate* _viewDelegate;

};


#endif //METAL_RENDERER_APP_DELEGATE_HPP
