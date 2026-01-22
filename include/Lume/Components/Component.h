#pragma once

#include <memory>
#include <optional>
#include <Lume/rendering/RenderContext.h>

class Actor;

class Component : public std::enable_shared_from_this<Component>
{
public:
    virtual ~Component() = default;
    explicit Component(const std::weak_ptr<Actor>& owner) : mOwner{ owner } {}

    virtual void start() {}
    virtual void update(float deltaTime) {}
    virtual void fixed_update(float fixedDeltaTime) {}
    virtual void on_destroy() {}

    // --- Owner ---
    [[nodiscard]] const auto& owner() const { return mOwner; }
    auto& owner() { return mOwner; }

    // --- Enable flag (getter/setter stile fluent) ---
    [[nodiscard]] const auto& enabled() const { return mEnabled; }
    auto& enabled()  { return mEnabled; }

    // --- Debug flag (getter/setter stile fluent) ---
    [[nodiscard]] const auto& debug() const { return mDebug; }
    auto& debug() { return mDebug; }

    // --- Render context ---
    [[nodiscard]] const auto& render_context() const { return mRenderContext; }
    auto& render_context() { return mRenderContext; }

protected:
    std::weak_ptr<Actor> mOwner = {};
    std::optional<RenderContext> mRenderContext = {};
    bool mEnabled = true;
    bool mDebug = false;
};