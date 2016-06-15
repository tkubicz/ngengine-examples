cmake_minimum_required (VERSION 3.0)

project(nge-examples)

# Do not build tests by default
set (BUILD_TESTS false)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../ngengine ngengine)

add_subdirectory(ngengine-blank)
add_subdirectory(ngengine-colored-triangle)
add_subdirectory(ngengine-ripple-deformation)
add_subdirectory(ngengine-subdivision-geometry)
add_subdirectory(ngengine-subdivision-geometry-instanced)
add_subdirectory(ngengine-simple-image-loader)
add_subdirectory(ngengine-waterexample)