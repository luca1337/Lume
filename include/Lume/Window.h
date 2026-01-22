#pragma once

#include <functional>
#include <memory>

#include <SDL3/SDL.h>

#include <Lume/Export.h>

class PostProcessing;

struct WindowProps
{
	unsigned mWidth = {};
	unsigned mHeight = {};
};

class RENDER_API Window
{
public:
	Window(unsigned width, unsigned height, unsigned depth = 32, unsigned majorVersion = 4, unsigned minorVersion = 6);
	Window(const Window &) = delete;
	Window(Window &&) = delete;

	auto update(const std::function<void(float)> &renderHook, const std::function<void(float)> &fixedRenderHook, const std::function<void(float)>& renderGuiHook = nullptr) -> void;
	auto close_window() -> void;

	[[nodiscard]] auto props() const { return mProps; }
	[[nodiscard]] auto is_open() const { return mIsOpen; }

	[[nodiscard]] auto get_gl_context() const { return mSdlGlContext; }
	[[nodiscard]] const auto &get_key_state() const { return mKeys; }

	auto set_post_processing_enabled(const bool enabled) { mIsPostProcessingEnabled = enabled; }
	auto set_post_processing(const std::shared_ptr<PostProcessing> &postProcessing) { mPostProcessing = postProcessing; }

	float mDeltaTime = {};
	float mAlpha = { };

private:
	WindowProps mProps = {};
	SDL_Window *mSdlWindowHandle = {};
	SDL_GLContext mSdlGlContext = {};

	std::shared_ptr<PostProcessing> mPostProcessing = {};

	bool mIsOpen = {};
	bool mIsPostProcessingEnabled = {};
	const unsigned char *mKeys = {};

private:
	auto render_scene_without_post_processing(const std::function<void(float)>& renderHook) const -> void;
};