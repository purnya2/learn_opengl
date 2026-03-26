cmake_minimum_required(VERSION 3.15)
project(learn_opengl    VERSION 1.0
                        DESCRIPTION "Texture"
                        LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_SUBDIRECTORY(external/glfw)
find_package(OpenGL REQUIRED)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE include)

ADD_EXECUTABLE(main
    src/main.cpp
    src/glad.c
    src/shader.cpp

)
target_include_directories(main PRIVATE include)

target_link_libraries(main
    PRIVATE glfw
    PRIVATE glm
    PRIVATE OpenGL::GL
    PRIVATE stb
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders"
        "$<TARGET_FILE_DIR:main>/shaders"
    COMMENT "Creating symlink to shader directory"
    VERBATIM
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/textures"
        "$<TARGET_FILE_DIR:main>/textures"
    COMMENT "Creating copy to texture directory"
    VERBATIM
)
