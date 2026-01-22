#include <iostream>

#include <Lume/Logger.h>
#include <Lume/Window.h>
#include <Lume/Rendering/Debug/DebugDrawer.h>
#include <Lume/rendering/PostProcess/PostProcessing.h>

Window::Window(const unsigned width, const unsigned height, const unsigned depth, const unsigned majorVersion, const unsigned minorVersion) : mProps{ .mWidth = width, .mHeight = height }
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "SDL error: " << SDL_GetError() << '\n';
		close_window();
		return;
	}

	mSdlWindowHandle = SDL_CreateWindow("SDL2 Window", mProps.mWidth, mProps.mHeight, SDL_WINDOW_OPENGL);
	if (!mSdlWindowHandle)
	{
		SDL_Log("Window creation failed: %s", SDL_GetError());
		close_window();
		return;
	}

	mSdlGlContext = SDL_GL_CreateContext(mSdlWindowHandle);
	if (!mSdlGlContext)
	{
		SDL_Log("OpenGL context creation failed: %s", SDL_GetError());
		close_window();
		return;
	}

	SDL_GL_SetSwapInterval(1); // Disable VSync

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
	{
		SDL_Log("Failed to initialize GLAD");
		close_window();
		return;
	}

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mKeys = reinterpret_cast<const unsigned char*>(SDL_GetKeyboardState(nullptr));

	mPostProcessing = std::make_shared<PostProcessing>(*this);

	mIsOpen = true;

	Logger::init();

	debug_drawer::init();
}

auto Window::update(const std::function<void(float)>& renderHook, const std::function<void(float)>& fixedRenderHook, const std::function<void(float)>& renderGuiHook) -> void
{
	auto prev_time = SDL_GetPerformanceCounter();
	float accumulator = 0.0f;
	constexpr float fixed_delta_time = 1.0f / 60.0f;

	while (mIsOpen)
	{
		auto ev = SDL_Event{};
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_EVENT_QUIT)
			{
				close_window();
			}
			else if (ev.type == SDL_EVENT_WINDOW_RESIZED)
			{
				int new_width = {};
				int new_height = {};
				SDL_GetWindowSize(mSdlWindowHandle, &new_width, &new_height);

				mProps.mWidth = new_width;
				mProps.mHeight = new_height;
			}
		}

		const auto current_time = SDL_GetPerformanceCounter();
		mDeltaTime = static_cast<float>(current_time - prev_time) / SDL_GetPerformanceFrequency();
		prev_time = current_time;

		accumulator += mDeltaTime;
		while (accumulator >= fixed_delta_time)
		{
			if (fixedRenderHook)
			{
				fixedRenderHook(fixed_delta_time);
			}

			accumulator -= fixed_delta_time;
		}

		mAlpha = accumulator / fixed_delta_time;

		if (mIsPostProcessingEnabled && mPostProcessing)
		{
			mPostProcessing->render_to_scene(mDeltaTime, [&] { render_scene_without_post_processing(renderHook); });
		}
		else
		{
			render_scene_without_post_processing(renderHook);
			// .. render_gui(render_gui_hook);
		}

		SDL_GL_SwapWindow(mSdlWindowHandle);
	}
}

auto Window::render_scene_without_post_processing(const std::function<void(float)>& renderHook) const -> void
{
	glViewport(0, 0, static_cast<GLsizei>(mProps.mWidth), static_cast<GLsizei>(mProps.mHeight));

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1.0f);

	if (renderHook != nullptr)
	{
		renderHook(mDeltaTime);
	}
}

auto Window::close_window() -> void
{
	if (mSdlWindowHandle)
	{
		SDL_DestroyWindow(mSdlWindowHandle);
		mSdlWindowHandle = nullptr;
	}

	SDL_Quit();
	mIsOpen = false;
}
