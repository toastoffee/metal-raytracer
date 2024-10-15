/**
  ******************************************************************************
  * @file           : app_delegate.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/14
  ******************************************************************************
  */



#include "app_delegate.hpp"

AppDelegate::~AppDelegate() {

}

NS::Menu *AppDelegate::createMenuBar() {
    return nullptr;
}

void AppDelegate::applicationWillFinishLaunching(NS::Notification *pNotification) {
    NS::Menu* menu = createMenuBar();
    auto app = reinterpret_cast<NS::Application *>(pNotification->object());
    app->setMainMenu(menu);
    app->setActivationPolicy(NS::ActivationPolicy::ActivationPolicyRegular);
}

void AppDelegate::applicationDidFinishLaunching(NS::Notification *pNotification) {
    CGRect frame = (CGRect){{100.0, 100.0}, {512.0, 512.0}};

    _device = MTL::CreateSystemDefaultDevice();

    _viewDelegate = new ViewDelegate(_device);

    _mtkView = MTK::View::alloc()->init(frame, _device);
    _mtkView->setColorPixelFormat(MTL::PixelFormat::PixelFormatRGBA8Unorm_sRGB);
    _mtkView->setClearColor( MTL::ClearColor::Make(0.0, 0.0, 0.0, 1.0) );
    _mtkView->setDelegate(_viewDelegate);

    _window = NS::Window::alloc()->init(
            frame,
            NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
            NS::BackingStoreBuffered,
            false);
    _window->setContentView(_mtkView);
    _window->setTitle(NS::String::string( "Ray Tracer", NS::StringEncoding::UTF8StringEncoding ));
    _window->makeKeyAndOrderFront(nullptr);

    auto app = reinterpret_cast<NS::Application *>(pNotification->object());
    app->activateIgnoringOtherApps(true);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application *pSender) {
    return true;
}

