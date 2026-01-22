#include <Lume/Utils.h>
#include <Lume/rendering/PostProcess/PostProcessing.h>

PostProcessing::PostProcessing(const Window& window) : mWindow(window)
{
    const auto quad_vertices = std::vector
    {
        -1.0f, -1.0f,  0.0f, 0.0f,
        -1.0f, 1.0f,  0.0f, 1.0f,
         1.0f, 1.0f,  1.0f, 1.0f,

        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, 1.0f,  1.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f
    };

    utils::gl::GenerateVertexBuffer(mVao, mVbo, quad_vertices);
    utils::gl::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
    utils::gl::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

    const auto width = mWindow.props().mWidth;
    const auto height = mWindow.props().mHeight;

    utils::gl::GenerateFrameBufferObject(width, height, mFbo, mTextureColorBuffer);
}
