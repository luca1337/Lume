#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(float left, float right, float bottom, float top);

	[[nodiscard]] const auto& position() const { return mPosition; }
	auto& position() { return mPosition; }

	[[nodiscard]] auto rotation() const -> float { return mRotation; }
	auto rotation(const float rotation) -> void { mRotation = rotation; recalculate_view_matrix(); }

	auto set_zoom(const float zoom) { mZoom = zoom; }

	[[nodiscard]] auto get_projection_matrix() const -> glm::mat4 { return mProjectionMatrix; }
	[[nodiscard]] auto get_view_matrix() const -> glm::mat4 { return mViewMatrix; }
	[[nodiscard]] auto get_view_projection_matrix() const -> glm::mat4 { return mViewProjectionMatrix; }

private:
	auto recalculate_view_matrix() -> void;

private:
	glm::mat4 mProjectionMatrix = {};
	glm::mat4 mViewMatrix = {};
	glm::mat4 mViewProjectionMatrix = {};

	glm::vec3 mPosition = {};
	float mRotation = {};
	float mZoom = 1.0f;
};