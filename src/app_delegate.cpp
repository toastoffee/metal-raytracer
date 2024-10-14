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
    ApplicationDelegate::applicationDidFinishLaunching(pNotification);
}

bool AppDelegate::applicationShouldTerminateAfterLastWindowClosed(NS::Application *pSender) {
    return ApplicationDelegate::applicationShouldTerminateAfterLastWindowClosed(pSender);
}

