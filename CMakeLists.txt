cmake_minimum_required(VERSION 3.20)



if(POLICY CMP0091)
  cmake_policy (SET CMP0091 NEW)
endif()

project(OpenGLExample)
include(cmake/CPM.cmake)

set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_SOURCE_DIR}/_CPMCache")
set(CPM_USE_LOCAL_PACKAGES ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

message("CPM_SOURCE_CACHE=${CPM_SOURCE_CACHE}")
if ( NOT DEFINED CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING "Build type" )
endif ()
#CPMAddPackage(NAME glwf URL "https://github.com/glfw/glfw/archive/refs/heads/master.zip")
#CPMAddPackage(NAME glm URL "https://github.com/g-truc/glm/archive/refs/heads/master.zip")
CPMAddPackage("gh:glfw/glfw#master")
CPMAddPackage("gh:g-truc/glm#master")
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(glew)
add_subdirectory(GLExample)

