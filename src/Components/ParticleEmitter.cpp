#include <exception>
#include <Lume/Actor.h>
#include <Lume/ResourceManager.h>
#include <Lume/World.h>
#include <Lume/components/ParticleEmitter.h>
#include <Lume/components/Transform.h>
#include <Lume/rendering/Camera.h>
#include <Lume/rendering/Shader.h>

ParticleEmitter::ParticleEmitter(const std::shared_ptr<Actor>& owner, const Config& cfg) : Component{ owner }, mConfig{ cfg }
{
    if (!owner)
    {
        throw std::exception("Particle emitter must be attached to a valid Actor.");
    }

    const auto& cam = owner->get_world().get_camera();
    const auto& particle_shader = ResourceManager::get_from_cache<Shader>({ .mResourceType = ResourceType::Shader, .mName = "ParticleShader"});

    render_context() = {
        .mView = cam->get_view_matrix(),
        .mProjection = cam->get_projection_matrix(),
        .mShader = particle_shader.value()
    };

    const auto& transform = owner->transform();
    mParticleSystem = std::make_unique<ParticleSystem>(
        transform->get_position(),
        cfg.mAmount,
        cfg.mMinSpeed,
        cfg.mMaxSpeed,
        cfg.mMinLifeTime,
        cfg.mMaxLifeTime
    );

    mParticleSystem->set_emission_rate(cfg.mEmissionRate);

    if (cfg.mInitialBurst > 0)
    {
        mParticleSystem->emit_burst(cfg.mInitialBurst);
    }

    mParticleSystem->set_shape_type(cfg.mShape);
    mParticleSystem->expire() = cfg.mExpire;

    if (cfg.mOnFinishCallback)
    {
        mParticleSystem->set_on_finish_callback(cfg.mOnFinishCallback);
    }
}

auto ParticleEmitter::start() -> void
{
    mParticleSystem->reset();
    mParticleSystem->emitting() = true;
}

auto ParticleEmitter::update(const float deltaTime) -> void
{
    mParticleSystem->update(deltaTime);
}

auto ParticleEmitter::render(const RenderContext& ctx) -> void
{
    mParticleSystem->render(ctx);
}
