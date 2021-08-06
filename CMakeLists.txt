cmake_minimum_required(VERSION 3.19)

project(Modulus VERSION 0.0.1
                LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${Modulus_SOURCE_DIR}/cmake")
message(STATUS "CMake module path: ${CMAKE_MODULE_PATH}")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

  set(CMAKE_CXX_EXTENSIONS OFF)

  #IDE folder support
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  include(CTest)
endif()

find_package(SDL2 REQUIRED)
#Arch linux build issues
if("${SDL2_LIBRARIES}" STREQUAL "")
    message(WARNING "SDL2_LIBRARIES wasn't set, manually setting to SDL2::SDL2")
    set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

message(STATUS "SDL2 included at ${SDL2_LIBRARIES}")

find_package(SDL2_image REQUIRED)
message(STATUS "SDL2_image included at ${SDL2_IMAGE_LIBRARIES}")

find_package(OpenGL REQUIRED)
message(STATUS "OpenGL included at ${OPENGL_LIBRARIES}")

find_package(GLEW REQUIRED)
message(STATUS "GLEW included at ${GLEW_LIBRARIES}}")

find_package(GLM REQUIRED)
MESSAGE(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

find_package(Freetype REQUIRED)
include_directories(/usr/include/freetype2)
message(STATUS "Freetype included at ${FREETYPE_LIBRARIES}")

cmake_policy(SET CMP0012 NEW)
find_package(assimp REQUIRED)
#if(ASSIMP_FOUND)
#	include_directories(${ASSIMP_INCLUDE_DIR})
#endif()
message(STATUS "Assimp included at ${ASSIMP_LIBRARIES}")

add_subdirectory(src)

add_subdirectory(bin)

#Set external resource paths
if(WIN32)
	target_compile_definitions( modulus PUBLIC ASSET_PATH="${CMAKE_CURRENT_BINARY_DIR}\\bin\\assets\\")
	target_compile_definitions( modulus PUBLIC SHADER_PATH="${CMAKE_CURRENT_BINARY_DIR}\\bin\\shaders\\")
else()
	target_compile_definitions( modulus PUBLIC ASSET_PATH="${CMAKE_CURRENT_BINARY_DIR}/bin/assets/")
	target_compile_definitions( modulus PUBLIC SHADER_PATH="${CMAKE_CURRENT_BINARY_DIR}/bin/shaders/")
endif()

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
  add_subdirectory(test)
endif()
