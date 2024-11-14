cmake_minimum_required(VERSION 3.27)
project(metal_renderer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(metal-cmake)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(${PROJECT_SOURCE_DIR}/third_party/assimp/include)

link_directories(third_party/assimp/lib)

add_executable(${PROJECT_NAME}
        main.cpp

        # stbi
        third_party/stbi/stb_image.cpp

        src/app_delegate.cpp
        src/app_delegate.hpp
        src/view_delegate.cpp
        src/view_delegate.hpp
        src/renderer.cpp
        src/renderer.hpp
        src/shader_tool.cpp
        src/shader_tool.hpp
        src/shader_types.hpp
        src/matrix_tool.cpp
        src/matrix_tool.hpp
        src/mesh_tool.cpp
        src/mesh_tool.hpp
        src/mesh.cpp
        src/mesh.hpp
)

target_link_libraries(${PROJECT_NAME} METAL_CPP assimp)
