#pragma once

#include <Lume/Color.h>
#include <memory>
#include <string>
#include <glm/glm.hpp>

class Transform;
class Shader;
class Texture;
struct RenderContext;

class Sprite
{
public:
	~Sprite();
	Sprite();
	Sprite(const Sprite& other);
	explicit Sprite(const std::string& texture_path);
	explicit Sprite(unsigned int width, unsigned int height, const Color& c);

	void render(const RenderContext& ctx, const Transform& tf) const;

	[[nodiscard]] const auto& sprite_color() const { return mColor; }
	auto& sprite_color() { return mColor; }

	[[nodiscard]] const auto& size() const { return mSize; }
	auto& size() { return mSize; }

	[[nodiscard]] const auto& sprite_texture() const { return mTexture; }
	auto& sprite_texture() { return mTexture; }

private:
	unsigned int mVao = {};
	unsigned int mVbo = {};

	std::shared_ptr<Texture> mTexture = {};
	Color mColor = {};
	glm::vec2 mSize = {};
};
