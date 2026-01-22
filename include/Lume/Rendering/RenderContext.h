#pragma once

#include <glm/glm.hpp>
#include <memory>

struct RenderContext
{
	glm::mat4 mView = {};
	glm::mat4 mProjection = {};
	std::shared_ptr<class Shader> mShader = {};
	bool mEnableDebug = {};
};