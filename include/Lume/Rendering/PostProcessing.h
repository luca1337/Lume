#pragma once

#include <functional>
#include <memory>
#include <Lume/Window.h>
#include <Lume/rendering/Shader.h>

class RENDER_API PostProcessing
{
public:
    virtual ~PostProcessing() = default;
    explicit PostProcessing(const Window& window);

    virtual auto render_to_scene(const float deltaTime, const std::function<void()>& renderSceneHook, const std::function<void()>& renderGuiHook = nullptr) -> void
    {
		// Render game scene to Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

        // Render scene as normal without GUI
        renderSceneHook();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

		// Apply post-processing to the game scene (if it exists), otherwise render on a flat texture and then apply effects.
        #pragma region SHADER_LOGIC

		if (mPpShader)
		{
			mPpShader->bind();
		}
        
        #pragma endregion

        glBindVertexArray(mVao);
        glBindBuffer(GL_ARRAY_BUFFER, mVbo);
        glBindTexture(GL_TEXTURE_2D, mTextureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

		// Render GUI on top of the post-processed scene to avoid being affected by post-processing effects
        if (renderGuiHook)
        {
	        renderGuiHook();
        }
    }

protected:
    const Window& mWindow;
    GLuint mVao, mVbo, mFbo, mTextureColorBuffer;
    std::shared_ptr<Shader> mPpShader = {};
};
