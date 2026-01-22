#include <Lume/Sprite.h>
#include <Lume/Texture.h>
#include <Lume/Utils.h>
#include <Lume/components/Transform.h>
#include <Lume/glad/glad.h>
#include <Lume/rendering/RenderContext.h>
#include <Lume/rendering/Shader.h>

const auto quad_vertices = std::vector{
    0.0f, 1.0f,       0.0f, 1.0f,
    0.0f, 0.0f,       0.0f, 0.0f,
    1.0f, 0.0f,       1.0f, 0.0f,

    0.0f, 1.0f,       0.0f, 1.0f,
    1.0f, 0.0f,       1.0f, 0.0f,
    1.0f, 1.0f,       1.0f, 1.0f
};

Sprite::~Sprite()
{
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
}

Sprite::Sprite()
{
    utils::gl::GenerateVertexBuffer(mVao, mVbo, quad_vertices);
    utils::gl::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    utils::gl::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));
}

Sprite::Sprite(const Sprite& other)
{
    utils::gl::GenerateVertexBuffer(mVao, mVbo, quad_vertices);
    utils::gl::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    utils::gl::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    mTexture = other.mTexture;
    mSize = other.mSize;
}

Sprite::Sprite(const std::string& texture_path)
{
	utils::gl::GenerateVertexBuffer(mVao, mVbo, quad_vertices);
	utils::gl::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
	utils::gl::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

	mTexture = std::make_shared<Texture>(texture_path);
	mSize = { static_cast<float>(mTexture->get_width()), static_cast<float>(mTexture->get_height()) };
}

Sprite::Sprite(const unsigned int width, const unsigned int height, const Color& c)
{
    utils::gl::GenerateVertexBuffer(mVao, mVbo, quad_vertices);
    utils::gl::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    utils::gl::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    mTexture = std::make_shared<Texture>(width, height, c);
    mSize = { static_cast<float>(mTexture->get_width()), static_cast<float>(mTexture->get_height()) };
}

auto Sprite::render(const RenderContext& ctx, const Transform& tf) const -> void
{
    auto model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(tf.get_position(), 0.0f));
    model = glm::rotate(model, tf.get_rotation(), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(mSize * tf.get_scale(), 1.0f));

    ctx.mShader->bind();

    if (mTexture)
    {
        glBindTexture(GL_TEXTURE_2D, mTexture->get_id());
    }

    ctx.mShader->set_matrix("model", model);
    ctx.mShader->set_matrix("view", ctx.mView);
    ctx.mShader->set_matrix("projection", ctx.mProjection);
    ctx.mShader->set_float("alpha", 1.0f);

    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);

    glPolygonMode(GL_FRONT_AND_BACK, ctx.mEnableDebug ? GL_LINE : GL_FILL);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (ctx.mEnableDebug)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    ctx.mShader->unbind();
}