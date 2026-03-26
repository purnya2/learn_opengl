cmake_minimum_required(VERSION 3.15)
project(learn_opengl    VERSION 1.0
                        DESCRIPTION "Shader"
                        LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

ADD_SUBDIRECTORY(external/glfw)
find_package(OpenGL REQUIRED)

ADD_EXECUTABLE(main
    src/main.cpp
    src/glad.c
    src/shader.cpp

)
target_include_directories(main PRIVATE include)

target_link_libraries(main
    PRIVATE glfw
    PRIVATE OpenGL::GL
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders"
        "$<TARGET_FILE_DIR:main>/shaders"
    COMMENT "Creating symlink to shader directory"
    VERBATIM
)
