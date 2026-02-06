cmake_minimum_required(VERSION 3.16)

project(ImageFilteringCLI)

set(CMAKE_CXX_STANDARD 17)

include_directories(external/stb)

add_executable(ImageFilteringCLI
    src/main.cpp
    src/Image.cpp
    src/filters/Filters.cpp
    src/pipeline/Pipeline.cpp
    src/pipeline/Registry.cpp
)
