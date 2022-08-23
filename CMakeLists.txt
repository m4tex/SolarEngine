cmake_minimum_required(VERSION 3.22)
project(SolarEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(SolarEngine src/main.cpp include/engine-methods.h src/engine-methods.cpp include/engine-structures.h)

target_link_libraries(SolarEngine -manifest:embed -manifestinput:"${PROJECT_SOURCE_DIR}/DeclareDPIAware.manifest")