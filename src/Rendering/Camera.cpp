#include <Lume/rendering/Camera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Camera::Camera(const float left, const float right, const float bottom, const float top)
	: mProjectionMatrix{ glm::ortho(left, right, bottom, top, -1.0f, 1.0f) }, mViewMatrix{ glm::mat4(1.0f) }
{
	recalculate_view_matrix();
}

auto Camera::recalculate_view_matrix() -> void
{
	const auto translation = glm::translate(glm::mat4(1.0f), mPosition);
	const auto rotation = glm::rotate(glm::mat4(1.0f), mRotation, glm::vec3{ 0.0f, 0.0f, 1.0f });
	const auto scale = glm::scale(glm::mat4(1.0f), glm::vec3{ mZoom, mZoom, 1.0f });
	const auto transform = translation * rotation * scale;

	mViewMatrix = glm::inverse(transform);
	mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
}