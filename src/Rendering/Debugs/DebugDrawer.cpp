#include <memory>
#include <vector>

#include <Lume/Logger.h>
#include <Lume/ResourceManager.h>
#include <Lume/Utils.h>
#include <Lume/rendering/Camera.h>
#include <Lume/rendering/Shader.h>
#include <Lume/Rendering/Debug/DebugDrawer.h>

namespace debug_drawer
{
	namespace
	{
		GLuint mVao = 0, mVbo = 0;
		std::vector<vertex> mVertices = {};
		std::shared_ptr<Shader> mShader = {};
	}

	auto init() -> void
	{
		utils::gl::GenerateVertexBuffer(mVao, mVbo, std::vector<vertex>{}, GL_DYNAMIC_DRAW);

		utils::gl::UploadVertexAttribute(0, 2, sizeof(vertex), 0, reinterpret_cast<void*>(offsetof(vertex, position)));
		utils::gl::UploadVertexAttribute(1, 4, sizeof(vertex), 1, reinterpret_cast<void*>(offsetof(vertex, color)));

		const auto& line_shader = ResourceManager::get_from_cache<Shader>({ .mResourceType = ResourceType::Shader, .mName = "LineShader" });
		if (!line_shader.has_value())
		{
			LOG_WARN("Line Shader could not be found.");
			return;
		}

		mShader = line_shader.value();
	}

	auto add_line(const glm::vec2 start, const glm::vec2 end, const glm::vec4 color) -> void
	{
		mVertices.push_back({ .position = start, .color = color });
		mVertices.push_back({ .position = end, .color = color });
	}

	auto add_rect(const glm::vec4& rect, const glm::vec4 color) -> void
	{
		const auto p1 = glm::vec2{ rect.x, rect.y };
		const auto p2 = glm::vec2{ rect.x + rect.z, rect.y };
		const auto p3 = glm::vec2{ rect.x + rect.z, rect.y + rect.w };
		const auto p4 = glm::vec2{ rect.x,rect.y + rect.w };

		add_line(p1, p2, color);
		add_line(p2, p3, color);
		add_line(p3, p4, color);
		add_line(p4, p1, color);
	}

	auto render(const Camera& cam) -> void
	{
		if (mVertices.empty()) { return; }

		mShader->bind();
		mShader->set_matrix("view", cam.get_view_matrix());
		mShader->set_matrix("projection", cam.get_projection_matrix());

		utils::gl::UpdateBufferData(mVbo, mVertices, GL_DYNAMIC_DRAW);

		glBindVertexArray(mVao);
		glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(mVertices.size()));

		mVertices.clear();
		glBindVertexArray(0);
	}
}
