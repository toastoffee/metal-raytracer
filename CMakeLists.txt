cmake_minimum_required(VERSION 3.27)
project(metal_renderer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(metal-cmake)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME}
        main.cpp
        src/app_delegate.cpp
        src/app_delegate.hpp
        src/view_delegate.cpp
        src/view_delegate.hpp
        src/renderer.cpp
        src/renderer.hpp
        src/shader_tool.cpp
        src/shader_tool.hpp
        src/shader_types.hpp
)

target_link_libraries(${PROJECT_NAME} METAL_CPP)
