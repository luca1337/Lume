#pragma once

#include <Lume/Logger.h>

#include <vector>
#include <Lume/glad/glad.h>

namespace utils::gl
{
    [[nodiscard]] inline auto GenerateOpenglTexture(const GLsizei width, const GLsizei height, const void *pixels, const GLint internalFormat = GL_RGBA,
                                                    const GLenum format = GL_RGBA, const GLint wrapS = GL_REPEAT, const GLint wrapT = GL_REPEAT, const bool generateMipMaps = false) -> GLuint
    {
        auto texture_id = GLuint{};

        glGenTextures(1, &texture_id);
        glActiveTexture(texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

        if (generateMipMaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texture_id;
    }

    inline auto GenerateVertexBuffer(GLuint &vao, GLuint &vbo, const std::vector<float> &vertices) -> void
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(vertices[0])), vertices.data(), GL_STATIC_DRAW);
    }

    inline auto UploadVertexAttribute(const GLuint slot, const GLint slotSize, const GLsizei stride, const GLuint attributeArrayIndex, const void *ptr) -> void
    {
        glVertexAttribPointer(slot, slotSize, GL_FLOAT, GL_FALSE, stride, ptr);
        glEnableVertexAttribArray(attributeArrayIndex);
    }

    inline auto GenerateFrameBufferObject(const GLuint width, const GLuint height, GLuint &fbo, GLuint &textureColorBuffer) -> void
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_CRITICAL("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
} // namespace utils::gl