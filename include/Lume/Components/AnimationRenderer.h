#pragma once

#include <Lume/components/Component.h>
#include <Lume/IRenderable.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct RenderContext;
class Sprite;

class AnimationRenderer final : public Component, public IRenderable
{
public:
    struct AnimationClipFrame
    {
		std::shared_ptr<Sprite> mFrameSprite = {};
        float mDuration = 0.1f;
    };

    struct AnimationClip
    {
        std::vector<AnimationClipFrame> mFrames = {};
    };

    ~AnimationRenderer() override = default;
    explicit AnimationRenderer(const std::shared_ptr<Actor>& owner);

    auto update(float deltaTime) -> void override;
    auto render(const RenderContext& ctx) -> void override;

    auto add_clip(const std::string& name, const AnimationClip& clip) -> void;
    auto increment_frame(const std::string& name = "") -> void;
    auto play(const std::string& name, bool loop = true) -> void;
    auto has_finished_playing() const { return !mLoop && !mIsPlaying; }
    auto get_current_playing_animation() const { return mCurrentAnimation; }
    auto stop() -> void;

    [[nodiscard]] auto current_sprite() const -> std::shared_ptr<Sprite>;

private:
    std::unordered_map<std::string, AnimationClip> mAnimations = {};
    std::string mCurrentAnimation = {};
    int mCurrentFrame = {};
    float mTimer = {};
    bool mIsPlaying = {};
    bool mLoop = true;
};
