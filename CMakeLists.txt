cmake_minimum_required(VERSION 3.22)
project(SolarEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(SolarEngine src/main.cpp main.manifest include/Renderer.h src/Renderer.cpp src/VertexBuffer.cpp include/VertexBuffer.h src/IndexBuffer.cpp include/IndexBuffer.h src/VertexArray.cpp include/VertexArray.h include/VertexBufferLayout.h src/Shader.cpp include/Shader.h include/Debugging.h src/vendor/stb_image/stb_image.h src/Texture.cpp include/Texture.h src/vendor/stb_image/stb_image.cpp src/vendor/imgui/imgui_draw.cpp src/vendor/imgui/imgui_demo.cpp src/vendor/imgui/imgui.cpp src/vendor/imgui/imgui_tables.cpp src/vendor/imgui/imgui_widgets.cpp src/vendor/imgui/imgui_impl_glfw.cpp src/vendor/imgui/imgui_impl_glfw.h src/vendor/imgui/imgui_impl_opengl3.cpp src/vendor/imgui/imgui_impl_opengl3.h src/vendor/imgui/imgui_impl_opengl3_loader.h)

target_include_directories(SolarEngine PUBLIC include PUBLIC src/vendor)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(SolarEngine OpenGL::GL glfw GLEW)
