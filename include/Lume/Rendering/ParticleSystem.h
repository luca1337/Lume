#pragma once

#include <atomic>
#include <condition_variable>
#include <Lume/Enums.h>
#include <functional>
#include <memory>
#include <mutex>
#include <Lume/Sprite.h>
#include <thread>
#include <vector>
#include <Lume/glad/glad.h>
#include <glm/glm.hpp>

struct Particle
{
    glm::vec2 mPosition = {};
    glm::vec2 mScale = { 1.0f, 1.0f };
    glm::vec4 mColor = {};
    glm::vec2 mVelocity = {};
    float mSpeed = {};
    float mLifeTime = {};
    float mRotation = {};
    bool mIsActive = {};
    bool mShouldExpire = {};
};

class Shader;
class Texture;

auto direction_by_shape_type(ShapeType type) -> glm::vec2;

class ParticleSystem
{
    static constexpr float ParticleQuadVertices[] = {
        0.0f, 1.0f,       0.0f, 1.0f,
        0.0f, 0.0f,       0.0f, 0.0f,
        1.0f, 0.0f,       1.0f, 0.0f,

        0.0f, 1.0f,       0.0f, 1.0f,
        1.0f, 0.0f,       1.0f, 0.0f,
        1.0f, 1.0f,       1.0f, 1.0f
    };

public:
    ~ParticleSystem();
    ParticleSystem() = delete;
    ParticleSystem(glm::vec2 position, unsigned maxAmount, float minSpeed, float maxSpeed, float minLifeTime, float maxLifeTime);
    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem(ParticleSystem&&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;
    ParticleSystem& operator=(ParticleSystem&&) = delete;

    auto set_shape_type(const ShapeType shapeType) -> void { mShapeType = shapeType; }
    auto set_on_finish_callback(const std::function<void()>& callback) -> void { mOnFinishCallback = callback; }
    auto reset() -> void;

    [[nodiscard]] const auto& emitting() const { return mIsEmitting; }
    auto& emitting() { return mIsEmitting; }

    [[nodiscard]] const auto& expire() const { return mShouldExpire; }
    auto& expire() { return mShouldExpire; }

    auto set_emission_rate(float rate) -> void;
    auto emit_burst(unsigned count) -> void;

    auto randomize_particles_position_between_bounds(float xMin, float xMax, float yMin, float yMax) -> void;
    auto set_position(const glm::vec2& position) -> void;
    auto update(float deltaTime) -> void;
    auto render(const RenderContext& ctx) const -> void;

private:
    std::vector<Particle> mParticles = {};

    GLuint mQuadVao = {};
    GLuint mQuadVbo = {};
    GLuint mInstanceVbo = {};
    GLuint mColorsVbo = {};

    glm::vec2 mStartPos = {};

    ShapeType mShapeType = {};

    float mMinSpeed = {};
    float mMaxSpeed = {};
    float mMinLifeTime = {};
    float mMaxLifeTime = {};

    bool mIsEmitting = false;
    bool mShouldExpire = false;

    unsigned mMaxAmount = {};
    unsigned mNumActive = {};

    std::shared_ptr<Texture> mTexture = {};

    std::function<void()> mOnFinishCallback = {};

    float mEmissionRate = {};
    float mEmissionAccumulator = {};
    unsigned mBurstCount = {};

private:
    auto generate_buffers() -> void;
    auto initialize_particles() -> void;
    auto update_instance_buffer() const -> void;
};