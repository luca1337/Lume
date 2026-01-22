#pragma once

#include <memory>
#include <vector>
#include <Lume/components/Component.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform final : public Component
{
public:
    explicit Transform(const std::shared_ptr<Actor>& owner);

    auto set_parent(const std::shared_ptr<Transform>& parent) -> void;
    [[nodiscard]] auto get_parent() const -> std::shared_ptr<Transform> { return mParent.lock(); }
    [[nodiscard]] auto get_children() const -> const std::vector<std::shared_ptr<Transform>>& { return mChildren; }

    auto set_position(const glm::vec2& pos) -> void;
    [[nodiscard]] auto get_position() const -> const glm::vec2& { return mPosition; }

    auto set_local_position(const glm::vec2& pos) -> void;
    [[nodiscard]] auto get_local_position() const -> const glm::vec2& { return mLocalPosition; }

    auto set_rotation(float rot) -> void;
    [[nodiscard]] auto get_rotation() const -> float { return mRotation; }

    auto set_euler_degrees(float deg) -> void;
    [[nodiscard]] auto get_euler_degrees() const -> float { return glm::degrees(mRotation); }

    auto set_scale(const glm::vec2& s) -> void;
    [[nodiscard]] auto get_scale() const -> const glm::vec2& { return mScale; }

private:
    auto update_child_position() -> void;
    auto update_child_rotation() -> void;
    auto update_child_scale() -> void;

    auto add_child(const std::shared_ptr<Transform>& child) -> void;
    auto remove_child(const std::shared_ptr<Transform>& child) -> void;

    std::weak_ptr<Transform> mParent;
    std::vector<std::shared_ptr<Transform>> mChildren = {};

    glm::vec2 mPosition = { 0.0f, 0.0f };
    glm::vec2 mLocalPosition = { 0.0f, 0.0f };

    float mRotation = 0.0f;
    float mPreviousParentRotation = 0.0f;

    glm::vec2 mScale = { 1.0f, 1.0f };
    glm::vec2 mPreviousParentScale = { 1.0f, 1.0f };
};
