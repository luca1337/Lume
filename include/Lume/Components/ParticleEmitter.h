#pragma once

#include <Lume/components/Component.h>
#include <Lume/IRenderable.h>
#include <Lume/rendering/ParticleSystem.h>
#include <memory>
#include <functional>

class ParticleEmitter final : public Component, public IRenderable
{
public:
    struct Config
    {
        unsigned mAmount = 100;
        float mMinSpeed = 20.f;
        float mMaxSpeed = 100.f;
        float mMinLifeTime = 0.5f;
        float mMaxLifeTime = 1.5f;
        float mEmissionRate = 0.f;      // nuove API: rate in particelle/secondo (0=nessuna emissione continua)
        unsigned mInitialBurst = 0;     // quante particelle emettere all'inizio, opzionale
        ShapeType mShape = ShapeType::Circle;
        bool mExpire = {};
        std::function<void()> mOnFinishCallback = {};
    };

    explicit ParticleEmitter(const std::shared_ptr<Actor>& owner, const Config& cfg);

    auto start() -> void override;
    auto update(float deltaTime) -> void override;
    auto render(const RenderContext& ctx) -> void override;

    [[nodiscard]] auto& get_emitter() { return mParticleSystem; }
    [[nodiscard]] const auto& get_config() const { return mConfig; }

private:
    std::unique_ptr<ParticleSystem> mParticleSystem = {};
    Config mConfig = {};
};