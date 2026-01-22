#pragma once

#include <Lume/IResource.h>
#include <string_view>
#include <Lume/glad/glad.h>
#include <glm/glm.hpp>

class RENDER_API Shader final : public IResource
{
public:
    Shader() = default;
    Shader(std::string_view vertex, std::string_view fragment);

    auto bind() const { glUseProgram(mProgramId); }
    static auto unbind() { glUseProgram(0); }

    auto set_matrix(const std::string_view name, const glm::mat4& m) const { glUniformMatrix4fv(glGetUniformLocation(mProgramId, name.data()), 1, false, &m[0][0]); }
    auto set_float(const std::string_view name, const float value) const { glUniform1f(glGetUniformLocation(mProgramId, name.data()), value); }
    auto set_vec3(const std::string_view name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(mProgramId, name.data()), 1, &value[0]); }
    auto set_vec2(const std::string_view name, const glm::vec2& value) const { glUniform2fv(glGetUniformLocation(mProgramId, name.data()), 1, &value[0]); }
    auto set_int(const std::string_view name, const int value) const { glUniform1i(glGetUniformLocation(mProgramId, name.data()), value); }
    auto set_uint(const std::string_view name, const uint32_t value) const { glUniform1ui(glGetUniformLocation(mProgramId, name.data()), value); }
    auto set_bool(const std::string_view name, const bool value) const { glUniform1i(glGetUniformLocation(mProgramId, name.data()), value); }

    [[nodiscard]] auto get_id() const { return mProgramId; }

private:
    GLuint mProgramId = {};
};
