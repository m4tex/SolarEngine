cmake_minimum_required(VERSION 3.22)
project(SolarEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(SolarEngine WIN32 src/main.cpp include/engine-methods.h src/engine-methods.cpp include/engine-structures.h main.manifest include/debug-console.h src/debug-console.cpp)