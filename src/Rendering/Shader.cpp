#include <filesystem>
#include <format>
#include <fstream>
#include <source_location>
#include <span>
#include <sstream>
#include <Lume/Logger.h>
#include <Lume/rendering/Shader.h>

auto read_shader(const std::string& path)
{
    auto shader_file = std::ifstream{ path };
    if (!shader_file.is_open())
    {
        const auto location = std::source_location::current();
        const auto error_msg = std::format("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {} [{}:{}]", path, location.file_name(), location.line());
        LOG_CRITICAL(error_msg);
    }

    auto shader_stream = std::stringstream{};
    shader_stream << shader_file.rdbuf();
    shader_file.close();
    return shader_stream.str();
}

auto compile_shader(const std::span<const char> shader_source, const GLenum shader_type)
{
    const auto shader = glCreateShader(shader_type);
    const auto shader_source_ptr = shader_source.data();
    glShaderSource(shader, 1, &shader_source_ptr, nullptr);
    glCompileShader(shader);

    GLint success = {};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        GLchar info_log[512] = {};
        glGetShaderInfoLog(shader, sizeof info_log, nullptr, info_log);

        const auto location = std::source_location::current();
        const auto error_msg = std::format("ERROR::SHADER::COMPILATION_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), info_log);
        LOG_CRITICAL(error_msg);
    }

    return shader;
}

Shader::Shader(const std::string_view vertex, const std::string_view fragment)
{
    const auto vertex_shader_code = read_shader(vertex.data());
    const auto fragment_shader_code = read_shader(fragment.data());

    const auto vertex_id = compile_shader(std::span(vertex_shader_code.data(), vertex_shader_code.size()), GL_VERTEX_SHADER);
    const auto fragment_id = compile_shader(std::span(fragment_shader_code.data(), fragment_shader_code.size()), GL_FRAGMENT_SHADER);

    // Shader program
    mProgramId = glCreateProgram();
    glAttachShader(mProgramId, vertex_id);
    glAttachShader(mProgramId, fragment_id);
    glLinkProgram(mProgramId);

    auto success = GLint{};
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);
    if (!success) 
    {
        GLchar info_log[512] = {};
        glGetProgramInfoLog(mProgramId, sizeof info_log, nullptr, info_log);

        const auto location = std::source_location::current();
        const auto error_message = std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED: [{}:{}]\n{}", location.file_name(), location.line(), info_log);
        LOG_CRITICAL(error_message);
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    LOG_INFO("GLSL Shaders successfully compiled!");
}
