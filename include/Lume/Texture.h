#pragma once

#include <Lume/Color.h>
#include <Lume/Export.h>
#include <string>
#include <vector>
#include <Lume/glad/glad.h>

class RENDER_API Texture
{
public:
	~Texture();
	Texture(const Texture&);
	explicit Texture(const std::string &file_path);
	explicit Texture(unsigned width, unsigned height, Color color);

	[[nodiscard]] auto get_id() const { return mTextureId; }

	[[nodiscard]] auto get_width() const { return mWidth; }
	[[nodiscard]] auto get_height() const { return mHeight; }

	[[nodiscard]] auto get_pixels() const -> const std::vector<unsigned char>& { return mPixelData; }
	[[nodiscard]] auto get_pixel(const int x, const int y) const -> const Color&;

	auto set_pixel(int x, int y, const Color& c) -> void;
	auto update_region(int x, int y, int width, int height) const -> void;
	auto update() const -> void;

private:
	GLuint mTextureId = {};
	unsigned mWidth = {};
	unsigned mHeight = {};

	std::vector<unsigned char> mPixelData; // RGBA buffer
};
