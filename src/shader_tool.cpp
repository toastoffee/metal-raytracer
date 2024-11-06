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

#include <stbi/stb_image.h>

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

MTL::Texture *ShaderTool::loadTexture(const char *textureFilePath, MTL::Device *device) {

    stbi_set_flip_vertically_on_load(false);
    int nrChannels, width, height;
    unsigned char *data = stbi_load(textureFilePath, &width, &height, &nrChannels, 3);


    MTL::TextureDescriptor* textureDesc = MTL::TextureDescriptor::alloc()->init();
    textureDesc->setWidth(width);
    textureDesc->setHeight(height);
    textureDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    textureDesc->setTextureType(MTL::TextureType2D);
    textureDesc->setStorageMode(MTL::StorageModeManaged);
    textureDesc->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead | MTL::ResourceUsageWrite);

    MTL::Buffer* texBuf = device->newBuffer(width * height * 4, MTL::ResourceStorageModeManaged);

    MTL::Texture* texture = texBuf->newTexture(textureDesc, 0, width * 4);

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int pxlIdx = i * height + j;
            memcpy((unsigned char *)texBuf->contents() + 4 * pxlIdx, data + 3 * pxlIdx, 3);

            ((unsigned char *)texBuf->contents())[4 * pxlIdx + 3] = 0xFF;
        }
    }

    texBuf->didModifyRange(NS::Range::Make(0, width * height * 4));

    textureDesc->release();

    return texture;
}
