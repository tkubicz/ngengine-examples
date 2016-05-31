cmake_minimum_required(VERSION 3.0)
project (nge-ripple-deformation)

# Set the current build profile.
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

# Specify the compilation flags for different build types.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -DUSE_GLFW -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -DUSE_GLFW -O3 -Wall")

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")

MESSAGE (STATUS "PROJECT BINARY DIR: " ${PROJECT_BINARY_DIR})
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/source/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/thirdparty/kaguya/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/thirdparty/pugixml/src/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/thirdparty/cppformat/")

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine subproject/nge)

set (CMAKE_INSTALL_PREFIX "../install")

set (EXTRA_LIBS ${EXTRA_LIBS} nge)
set (EXTRA_LIBS ${EXTRA_LIBS} GL)
set (EXTRA_LIBS ${EXTRA_LIBS} GLU)
set (EXTRA_LIBS ${EXTRA_LIBS} GLEW)
set (EXTRA_LIBS ${EXTRA_LIBS} glfw)
set (EXTRA_LIBS ${EXTRA_LIBS} z)
set (EXTRA_LIBS ${EXTRA_LIBS} lua5.2)
set (EXTRA_LIBS ${EXTRA_LIBS} cppformat)

# Add the executables
add_executable(${PROJECT_NAME} src/RippleDeformation.cpp src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

install (TARGETS ${PROJECT_NAME} DESTINATION bin)