#include <utility>
#include <Lume/Actor.h>
#include <Lume/ResourceManager.h>
#include <Lume/Sprite.h>
#include <Lume/World.h>
#include <Lume/components/AnimationRenderer.h>
#include <Lume/components/Transform.h>
#include <Lume/rendering/Camera.h>
#include <Lume/rendering/RenderContext.h>
#include <Lume/rendering/Shader.h>

AnimationRenderer::AnimationRenderer(const std::shared_ptr<Actor>& owner) : Component{ owner } 
{
	const auto& cam = owner->get_world().get_camera();
	const auto& sprite_shader = ResourceManager::get_from_cache<Shader>({ .mResourceType = ResourceType::Shader, .mName = "SpriteShader" });
	render_context() = { .mView = cam->get_view_matrix(), .mProjection = cam->get_projection_matrix(), .mShader = sprite_shader.value() };
}

void AnimationRenderer::update(const float deltaTime)
{
 	if (!mIsPlaying || mCurrentAnimation.empty()) { return; }

	const auto& [frames] = mAnimations[mCurrentAnimation];
	if (frames.empty()) { return; }

	mTimer += deltaTime;

	if (const auto& [frame_sprite, duration] = frames[mCurrentFrame]; mTimer >= duration)
	{
		mTimer = {};
 		++mCurrentFrame;

		if (std::cmp_greater_equal(mCurrentFrame, frames.size()))
		{
			if (mLoop)
			{
				mCurrentFrame = {};
			}
			else
			{
				mIsPlaying = false;
				mCurrentFrame = static_cast<int>(frames.size()) - 1;
			}
		}
	}
}

void AnimationRenderer::render(const RenderContext& ctx)
{
	if (const auto& sprite = current_sprite())
	{
		if (const auto& ow = owner().lock(); ow)
		{
			const auto& transform = ow->transform();
			sprite->render(ctx, *transform);
		}
	}
}

auto AnimationRenderer::add_clip(const std::string& name, const AnimationClip& clip) -> void
{
	if (mAnimations.contains(name))
	{
		LOG_WARN("AnimationRenderer::add_clip: Animation clip '{}' already exists", name);
		return;
	}

	mAnimations.emplace(name, clip);
}

auto AnimationRenderer::increment_frame(const std::string& name) -> void
{
	auto& [frames] = mAnimations[name.empty() ? mCurrentAnimation : name];

	if (frames.empty()) { return; }

	++mCurrentFrame;
	if (std::cmp_greater_equal(mCurrentFrame, frames.size()))
	{
		if (mLoop)
		{
			mCurrentFrame = {};
		}
		else
		{
			mIsPlaying = false;
			mCurrentFrame = static_cast<int>(frames.size()) - 1;
		}
	}
}

auto AnimationRenderer::play(const std::string& name, const bool loop) -> void
{
	if (!mAnimations.contains(name)) { return; }

	mCurrentAnimation = name;
	mCurrentFrame = 0;
	mTimer = 0.0f;
	mIsPlaying = true;
	mLoop = loop;
}

auto AnimationRenderer::stop() -> void
{
	mIsPlaying = false;
	mCurrentFrame = 0;
	mTimer = 0.0f;
}

auto AnimationRenderer::current_sprite() const -> std::shared_ptr<Sprite>
{
	const auto& [frames] = mAnimations.at(mCurrentAnimation);
	if (frames.empty()) { return {}; }

	return frames[mCurrentFrame].mFrameSprite;
}
