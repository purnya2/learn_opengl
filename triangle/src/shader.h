#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "glad/glad.h"
#include <stdexcept>
#include <string>
#include <string_view>

auto loadShader(std::string_view path) -> std::string;

class ShaderCompilationError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

class ShaderLoader {
public:
    // Public interface
    static unsigned int fromVertexFile(std::string_view path);
    static unsigned int fromFragmentFile(std::string_view path);

private:
    // Private implementation methods
    static void checkCompilationErrors(unsigned int shader, GLenum type);
    static unsigned int compileVertexShader(const std::string& source);
    static unsigned int compileFragmentShader(const std::string& source);
    static unsigned int compileShader(GLenum type, const std::string& source);
};

#endif // SHADER_LOADER_H
