#pragma once

#include <glm/vec3.hpp>

class Ray
{
public:
    ~Ray() = default;
    Ray() = default;
    Ray(const glm::vec3& origin, const glm::vec3& direction) : mOrigin{origin}, mDirection{direction} {}
    Ray(const Ray&) = default;
    Ray(Ray&&) = default;
    Ray& operator=(const Ray&) = default;
    Ray& operator=(Ray&&) = default;

    [[nodiscard]] auto origin() const -> glm::vec3 { return mOrigin;}
    [[nodiscard]] auto direction() const -> glm::vec3 { return mDirection; }
    [[nodiscard]] auto at(const float t) const -> glm::vec3 { return mOrigin + t * mDirection; }

private:
    glm::vec3 mOrigin = {};
    glm::vec3 mDirection = {};
};