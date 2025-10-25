cmake_minimum_required(VERSION 3.10)

project(silk VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include imgui src)

add_subdirectory(include/tinygltf)

set(SOURCES
        src/main.cpp

  include/glad/glad.c
        src/Rendering/Shader.cpp
        src/Rendering/Shader.hpp
        src/Core/Camera.cpp
        src/Core/Camera.hpp
        src/Core/Engine.cpp
        src/Core/Engine.hpp
        src/Core/EngineConfig.cpp
        src/Core/EngineConfig.hpp
        # include/stb/stb_image.c
)

# ImGui
set(IMGUI_SOURCES
  imgui/imgui.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/imgui_demo.cpp
  imgui/backends/imgui_impl_glfw.cpp
  imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(silk ${SOURCES} ${IMGUI_SOURCES})

find_package(OpenGL REQUIRED)


if(APPLE)
  message(STATUS "Building on macOS")

  find_library(COCOA_LIBRARY Cocoa REQUIRED)
  find_library(IOKIT_LIBRARY IOKit REQUIRED)
  find_library(COREVIDEO_LIBRARY CoreVideo REQUIRED)

  find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)

pkg_check_modules(GLM REQUIRED glm)

target_include_directories(silk PRIVATE ${GLFW_INCLUDE_DIRS})

target_link_directories(silk PRIVATE ${GLFW_LIBRARY_DIRS})

target_include_directories(silk PRIVATE ${GLM_INCLUDE_DIRS})

target_link_directories(silk PRIVATE ${GLM_LIBRARY_DIRS})

  target_link_libraries(silk
    ${GLFW_LIBRARIES}
    ${GLM_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${COCOA_LIBRARY}
    ${IOKIT_LIBRARY}
    ${COREVIDEO_LIBRARY}
    tinygltf
  )

else()
  message(STATUS "Building on Linux/Windows")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

pkg_check_modules(GLM REQUIRED glm)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})

  target_link_libraries(silk
    ${GLFW_LIBRARIES}
    ${GLM_LIBRARIES}
    ${OPENGL_gl_LIBRARY}
    tinygltf
    dl
  )
endif()
