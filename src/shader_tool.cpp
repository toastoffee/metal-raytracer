/**
  ******************************************************************************
  * @file           : ShaderTool.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/10/16
  ******************************************************************************
  */

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#include "shader_tool.hpp"

std::string ShaderTool::readSource(const char *filePath) {

    std::string source;

    std::ifstream sourceFile;
    sourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        sourceFile.open(filePath);

        std::stringstream fileStream;

        fileStream << sourceFile.rdbuf();

        sourceFile.close();

        source = fileStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    return source;
}

MTL::Library *ShaderTool::createLibrary(const char *shaderFilePath, MTL::Device *device) {
    using NS::StringEncoding::UTF8StringEncoding;

    NS::Error* error = nullptr;
    auto sourceCode = readSource(shaderFilePath);
    auto library = device->newLibrary(NS::String::string(sourceCode.c_str(), UTF8StringEncoding),
                                      nullptr, &error);
    if(!library) {
        __builtin_printf("%s", error->localizedDescription()->utf8String());
        assert(false);
    }
    return library;
}

MTL::RenderPipelineState *ShaderTool::loadShader(const char *shaderFilePath, MTL::Device *device) {
    auto library = createLibrary(shaderFilePath, device);

    MTL::Function* vertexFn = library->newFunction(NS::String::string("vertexMain", NS::StringEncoding::UTF8StringEncoding));
    MTL::Function* fragFn = library->newFunction(NS::String::string("fragmentMain", NS::StringEncoding::UTF8StringEncoding));

    MTL::RenderPipelineDescriptor* desc = MTL::RenderPipelineDescriptor::alloc()->init();
    desc->setVertexFunction(vertexFn);
    desc->setFragmentFunction(fragFn);
    desc->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatRGBA8Unorm_sRGB);

    NS::Error* error = nullptr;

    MTL::RenderPipelineState *pso = device->newRenderPipelineState(desc, &error);

    if ( !pso )
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    vertexFn->release();
    fragFn->release();
    library->release();
    desc->release();

    return pso;
}

MTL::ComputePipelineState *ShaderTool::loadComputeShader(const char *shaderFilePath, MTL::Device *device) {
    auto computeLibrary = createLibrary(shaderFilePath, device);

    MTL::Function* computeFunc = computeLibrary->newFunction(NS::String::string("computeMain", NS::UTF8StringEncoding));

    NS::Error* error = nullptr;

    MTL::ComputePipelineState *pso = device->newComputePipelineState(computeFunc, &error);

    if( !pso )
    {
        __builtin_printf( "%s", error->localizedDescription()->utf8String() );
        assert( false );
    }

    computeFunc->release();
    computeLibrary->release();

    return pso;
}
