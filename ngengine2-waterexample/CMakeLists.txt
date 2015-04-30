cmake_minimum_required(VERSION 2.8)
project (nge2-waterexample)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-g -std=c++11 -pthread -Wall)
endif()

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src")

MESSAGE (STATUS "PROJECT BINARY DIR: " ${PROJECT_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/../ngengine2/source/include/")
add_subdirectory (../ngengine2 subproject/nge2)

set (CMAKE_INSTALL_PREFIX "../install")

set (EXTRA_LIBS ${EXTRA_LIBS} nge2)
set (EXTRA_LIBS ${EXTRA_LIBS} GL)
set (EXTRA_LIBS ${EXTRA_LIBS} GLU)
set (EXTRA_LIBS ${EXTRA_LIBS} GLEW)
set (EXTRA_LIBS ${EXTRA_LIBS} glfw)
set (EXTRA_LIBS ${EXTRA_LIBS} z)

MESSAGE(STATUS "Libs: " ${EXTRA_LIBS})

# Add the executables
add_executable(nge2-waterexample 
	src/Background.cpp
	#src/Water.cpp
	src/WaterExample.cpp
	src/WaterTexture.cpp
	src/main.cpp)

target_link_libraries(nge2-waterexample ${EXTRA_LIBS})

install (TARGETS nge2-waterexample DESTINATION bin)