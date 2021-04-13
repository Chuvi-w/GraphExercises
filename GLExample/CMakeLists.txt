
find_package (OpenGL REQUIRED)

add_executable(GLGraph graph.cpp)

target_link_libraries(GLGraph PUBLIC glew_s glfw glm::glm )

