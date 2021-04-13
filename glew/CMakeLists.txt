

if(POLICY CMP0003)
cmake_policy (SET CMP0003 NEW)
endif()

if(POLICY CMP0042)
cmake_policy (SET CMP0042 NEW)
endif()


option (BUILD_UTILS "utilities" ON)
option (GLEW_REGAL "Regal mode" OFF)
option (GLEW_OSMESA "OSMesa mode" OFF)



find_package (OpenGL REQUIRED)

# X11 required except for Windows and Apple OSX platforms
if (NOT WIN32 AND NOT APPLE)
  find_package (X11)
endif()

if (WIN32)
set (GLEW_LIB_NAME glew32)
else ()
set (GLEW_LIB_NAME GLEW)
set (DLL_PREFIX lib)
endif ()

set (GLEW_LIBRARIES ${OPENGL_LIBRARIES} ${X11_LIBRARIES})

#set (GLEW_PUBLIC_HEADERS_FILES include/GL/wglew.h include/GL/glew.h include/GL/glxew.h)
add_library (glew_s STATIC src/glew.c)
target_include_directories(glew_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(glew_s PUBLIC GLEW_STATIC)
set_target_properties (glew_s PROPERTIES OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX lib)
target_compile_definitions(glew_s PUBLIC GLEW_NO_GLU)

#### Regal mode ####
if (GLEW_REGAL)
if (WIN32)
  set (REGAL_LIB_NAME regal32)
else ()
  set (REGAL_LIB_NAME Regal)
endif ()
set (GLEW_LIBRARIES ${REGAL_LIB_NAME})
target_compile_definitions(glew_s PRIVATE GLEW_REGAL)
endif ()

#### OSMesa mode ####

if (GLEW_OSMESA)
if (WIN32)
  set (OSMESA_LIB_NAME osmesa)
else ()
  set (OSMESA_LIB_NAME OSMesa)
endif ()
target_compile_definitions(glew_s PRIVATE GLEW_OSMESA)
set (GLEW_LIBRARIES ${OSMESA_LIB_NAME} ${OPENGL_LIBRARIES})
set (X11_LIBRARIES)
endif ()

#### EGL ####

if (GLEW_EGL AND UNIX)
target_compile_definitions(glew_s PRIVATE GLEW_EGL)
if (OpenGL::EGL)
  message (FATAL_ERROR "EGL library set but not found.")
endif()
set (GLEW_LIBRARIES ${OPENGL_LIBRARIES} ${OPENGL_egl_LIBRARY})
endif ()

#### GLEW ####
target_include_directories(glew_s PUBLIC ${GLEW_DIR}/include ${X11_INCLUDE_DIR})

if (MSVC)
# add options from visual studio project
target_compile_definitions (glew_s PRIVATE GLEW_STATIC VC_EXTRALEAN)
# kill security checks which are dependent on stdlib
target_compile_options (glew_s PRIVATE -GS-)

string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
elseif (WIN32 AND ((CMAKE_C_COMPILER_ID MATCHES "GNU") OR (CMAKE_C_COMPILER_ID MATCHES "Clang")))
# remove stdlib dependency on windows with GCC and Clang (for similar reasons
# as to MSVC - to allow it to be used with any Windows compiler)
target_compile_options (glew_s PRIVATE -fno-builtin -fno-stack-protector)
endif ()

target_link_libraries (glew_s PUBLIC ${GLEW_LIBRARIES} ${X11_LIBRARIES})
