#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Camera;

namespace debug_drawer
{
    struct vertex
    {
        glm::vec2 position = {};
        glm::vec4 color = {};
    };

    auto init() -> void;

    auto add_line(glm::vec2 start, glm::vec2 end, glm::vec4 color) -> void;
    auto add_rect(const glm::vec4& rect, glm::vec4 color) -> void;

    auto render(const Camera& cam) -> void;
}