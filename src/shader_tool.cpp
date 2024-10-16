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
