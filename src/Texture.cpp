#include <Lume/Logger.h>
#include <Lume/Texture.h>
#include <Lume/Utils.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Lume/stb_image.h>

Texture::~Texture()
{
    if (mTextureId != 0)
    {
        glDeleteTextures(1, &mTextureId);
        mTextureId = 0;
    }
}

Texture::Texture(const Texture& other)
    : mWidth(other.mWidth), mHeight(other.mHeight), mPixelData(other.mPixelData)
{
    glGenTextures(1, &mTextureId);
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixelData.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const std::string& file_path)
{
    int width, height, nr_channels;
    if (const auto data = stbi_load(file_path.c_str(), &width, &height, &nr_channels, 4))
    {
        mWidth = width;
        mHeight = height;
        mPixelData.assign(data, data + width * height * 4);

        mTextureId = utils::gl::GenerateOpenglTexture(static_cast<GLsizei>(mWidth), static_cast<GLsizei>(mHeight), data, GL_RGBA, GL_RGBA, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

        stbi_image_free(data);
    }
    else
    {
        LOG_CRITICAL("Failed to load texture");
    }
}

Texture::Texture(const unsigned width, const unsigned height, const Color color) : mWidth(width), mHeight(height), mPixelData(width * height * 4)
{
    for (unsigned i = 0; i < width * height; ++i)
    {
        mPixelData[static_cast<std::vector<unsigned char>::size_type>(i) * 4 + 0] = color.r;
        mPixelData[static_cast<std::vector<unsigned char>::size_type>(i) * 4 + 1] = color.g;
        mPixelData[static_cast<std::vector<unsigned char>::size_type>(i) * 4 + 2] = color.b;
        mPixelData[static_cast<std::vector<unsigned char>::size_type>(i) * 4 + 3] = color.a;
    }

    mTextureId = utils::gl::GenerateOpenglTexture(static_cast<GLsizei>(width), static_cast<GLsizei>(height), mPixelData.data(), GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, true);
}

auto Texture::get_pixel(const int x, const int y) const -> const Color&
{
	if (x < 0 || y < 0 || x >= static_cast<int>(mWidth) || y >= static_cast<int>(mHeight))
	{
        return Black;
	}

	const int flipped_y = mHeight - y - 1;
	const size_t index = (flipped_y * mWidth + x) * 4;
	return {.r = mPixelData[index + 0], .g = mPixelData[index + 1], .b = mPixelData[index + 2], .a = mPixelData[index + 3] };
}

auto Texture::set_pixel(const int x, const int y, const Color& c) -> void
{
    if (x < 0 || y < 0 || x >= static_cast<int>(mWidth) || y >= static_cast<int>(mHeight))
    {
        return;
    }

    const int flipped_y = mHeight - y - 1;
    const size_t index = (flipped_y * mWidth + x) * 4;

    mPixelData[index + 0] = c.r;
    mPixelData[index + 1] = c.g;
    mPixelData[index + 2] = c.b;
    mPixelData[index + 3] = c.a;
}

auto Texture::update_region(const int x, const int y, const int width, const int height) const -> void
{
    if (mTextureId == 0 || mPixelData.empty()) return;
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &mPixelData[(y * mWidth + x) * 4]);
}

auto Texture::update() const -> void
{
    if (mTextureId == 0 || mPixelData.empty()) return;
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(mWidth), static_cast<GLsizei>(mHeight), GL_RGBA, GL_UNSIGNED_BYTE, mPixelData.data());
}