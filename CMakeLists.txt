cmake_minimum_required(VERSION 3.22)
project(SolarEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(SolarEngine src/main.cpp main.manifest)
find_package(GLEW REQUIRED)
target_link_libraries(SolarEngine GLEW)
target_link_libraries(SolarEngine glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(SolarEngine OpenGL::GL)
