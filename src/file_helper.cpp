/**
  ******************************************************************************
  * @file           : file_helper.cpp
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

#include "file_helper.hpp"

std::string FileHelper::readSource(const char *filePath) {

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
