
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include <iostream>
#include <simd/simd.h>

#include "app_delegate.hpp"

#include "mesh_tool.hpp"

int main( int argc, char* argv[] )
{

    NS::AutoreleasePool* autoreleasePool = NS::AutoreleasePool::alloc()->init();

    AppDelegate appDelegate;

    NS::Application* sharedApplication = NS::Application::sharedApplication();
    sharedApplication->setDelegate(&appDelegate);
    sharedApplication->run();

    autoreleasePool->release();

    auto mesh = MeshTool::loadMesh("../static/bunny.obj");

    return 0;
}