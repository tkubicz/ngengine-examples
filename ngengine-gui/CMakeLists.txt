cmake_minimum_required(VERSION 3.0)
project (nge-gui)

# Set the current build profile.
if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Debug)
endif()

# Specify the compilation flags for different build types.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -DUSE_GLFW -g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -DUSE_GLFW -O3 -Wall")

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/source/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/thirdparty/kaguya/include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/thirdparty/pugixml/src/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine/thirdparty/cppformat/")

if ("${PROJECT_NAME}" STREQUAL "${CMAKE_PROJECT_NAME}")
	add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../ngengine ngengine)
endif()

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
add_executable(${PROJECT_NAME} src/BlankExample.cpp src/GUIRectangle.cpp src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})

install (TARGETS ${PROJECT_NAME} DESTINATION bin)