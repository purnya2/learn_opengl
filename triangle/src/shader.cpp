#include "shader.h"
#include <fstream>
#include <vector>
#include <filesystem>

auto loadShader(std::string_view path) -> std::string
{
    std::ifstream file(path.data(), std::ios::binary);
    if (!file.is_open()) {
        throw ShaderCompilationError("Failed to open shader file: " + std::string(path));
    }

    std::string content;
    auto size = std::filesystem::file_size(path);
    content.resize(size);
    file.read(content.data(), size);

    if (!file) {
        throw ShaderCompilationError("Failed to read shader file: " + std::string(path));
    }

    return content;
}

unsigned int ShaderLoader::fromVertexFile(std::string_view path)
{
    auto shaderSource = loadShader(path);
    return compileVertexShader(shaderSource);
}

unsigned int ShaderLoader::fromFragmentFile(std::string_view path)
{
    auto shaderSource = loadShader(path);
    return compileFragmentShader(shaderSource);
}

void ShaderLoader::checkCompilationErrors(unsigned int shader, GLenum type)
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLint logSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

        if (logSize > 0) {
            std::vector<char> infoLog(logSize + 1, '\0');
            glGetShaderInfoLog(shader, logSize, nullptr, infoLog.data());

            std::string errorMsg = "Shader compilation failed:\n";
            errorMsg += infoLog.data();

            glDeleteShader(shader);
            throw ShaderCompilationError(errorMsg);
        } else {
            glDeleteShader(shader);
            throw ShaderCompilationError("Shader compilation failed with no error log");
        }
    }
}

unsigned int ShaderLoader::compileVertexShader(const std::string& source)
{
    return compileShader(GL_VERTEX_SHADER, source);
}

unsigned int ShaderLoader::compileFragmentShader(const std::string& source)
{
    return compileShader(GL_FRAGMENT_SHADER, source);
}

unsigned int ShaderLoader::compileShader(GLenum type, const std::string& source)
{
    unsigned int shader = glCreateShader(type);

    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
    glCompileShader(shader);

    checkCompilationErrors(shader, type);

    return shader;
}
