#include <chrono>
#include <Lume/Enums.h>
#include <Lume/Logger.h>
#include <Lume/Random.h>
#include <Lume/Texture.h>
#include <Lume/Utils.h>
#include <Lume/rendering/Camera.h>
#include <Lume/rendering/ParticleSystem.h>
#include <Lume/rendering/RenderContext.h>
#include <Lume/rendering/Shader.h>

auto direction_by_shape_type(const ShapeType type) -> glm::vec2
{
	auto direction = glm::vec2{};

	switch (type)
	{
	case ShapeType::HalfCircle:
	{
		const auto random_angle = Rng::generate_random_number<float>(-glm::pi<float>(), glm::pi<float>());
		direction = { glm::cos(random_angle), glm::sin(random_angle) };
	}
	break;
	case ShapeType::Circle:
	{
		const auto random_angle = Rng::generate_random_number<float>(0.0f, 2 * glm::pi<float>());
		direction = { glm::cos(random_angle), glm::sin(random_angle) };
	}
	break;
	case ShapeType::None:
		direction = { Rng::generate_random_number(-1.0f, 1.0f), Rng::generate_random_number(-1.0f, 1.0f) };
		break;
	default:
		break;
	}

	return direction;
}

ParticleSystem::~ParticleSystem()
{
	glDeleteVertexArrays(1, &mQuadVao);
	glDeleteBuffers(1, &mQuadVbo);
	glDeleteBuffers(1, &mInstanceVbo);
	glDeleteBuffers(1, &mColorsVbo);
}

ParticleSystem::ParticleSystem(const glm::vec2 position, const unsigned maxAmount, const float minSpeed, const float maxSpeed, const float minLifeTime, const float maxLifeTime)
	: mStartPos{ position }, mShapeType{ ShapeType::Circle },
	mMinSpeed{ minSpeed }, mMaxSpeed{ maxSpeed }, mMinLifeTime{ minLifeTime }, mMaxLifeTime{ maxLifeTime }, mMaxAmount{ maxAmount }
{
	mParticles.resize(mMaxAmount);
	generate_buffers();
	initialize_particles();
}

auto ParticleSystem::reset() -> void
{
	PROFILE_SCOPE("Reset");

	mEmissionAccumulator = 0.0f;
	mNumActive = 0;
	mBurstCount = 0;

	for (auto& p : mParticles)
	{
		p.mPosition = mStartPos;
		p.mVelocity = direction_by_shape_type(mShapeType);
		p.mSpeed = Rng::generate_random_number(mMinSpeed, mMaxSpeed);
		p.mLifeTime = Rng::generate_random_number(mMinLifeTime, mMaxLifeTime);
		p.mColor = glm::vec4{
			Rng::generate_random_number(0.3f, 1.0f),
			Rng::generate_random_number(0.3f, 1.0f),
			Rng::generate_random_number(0.3f, 1.0f),
			1.0f
		};
		p.mIsActive = false;
		p.mShouldExpire = false;
	}

	update_instance_buffer();
}

auto ParticleSystem::set_emission_rate(const float rate) -> void
{
	mEmissionRate = rate;
}

auto ParticleSystem::emit_burst(const unsigned count) -> void
{
	mBurstCount += count;
}

auto ParticleSystem::randomize_particles_position_between_bounds(const float xMin, const float xMax, const float yMin, const float yMax) -> void
{
	for (auto& p : mParticles)
	{
		const auto new_x = mStartPos.x + Rng::generate_random_number(xMin, xMax);
		const auto new_y = mStartPos.y + Rng::generate_random_number(yMin, yMax);
		p.mPosition = glm::vec2{ new_x, new_y };
	}
	update_instance_buffer();
}

auto ParticleSystem::set_position(const glm::vec2& position) -> void
{
	mStartPos = position;

	for (auto& p : mParticles)
	{
		p.mPosition = position;
	}
	update_instance_buffer();
}

auto ParticleSystem::update(const float deltaTime) -> void
{
	if (!mIsEmitting) { return; }

	if (mBurstCount > 0)
	{
		PROFILE_SCOPE("Burst");

		while (mBurstCount > 0)
		{
			bool emitted = false;
			for (auto& p : mParticles)
			{
				if (!p.mIsActive && !p.mShouldExpire)
				{
					p.mIsActive = true;
					p.mPosition = mStartPos;
					p.mVelocity = direction_by_shape_type(mShapeType);
					p.mSpeed = Rng::generate_random_number(mMinSpeed, mMaxSpeed);
					p.mLifeTime = Rng::generate_random_number(mMinLifeTime, mMaxLifeTime);
					p.mColor = glm::vec4{
						Rng::generate_random_number(0.3f, 1.0f),
						Rng::generate_random_number(0.3f, 1.0f),
						Rng::generate_random_number(0.3f, 1.0f),
						1.0f
					};
					--mBurstCount;
					emitted = true;
					break;
				}
			}
			if (!emitted) break;
		}
	}

	if (mEmissionRate > 0.0f)
	{
		mEmissionAccumulator += deltaTime * mEmissionRate;
		int num_to_emit = static_cast<int>(mEmissionAccumulator);
		mEmissionAccumulator -= num_to_emit;

		for (auto& p : mParticles)
		{
			if (num_to_emit <= 0) break;
			if (!p.mIsActive && !p.mShouldExpire)
			{
				p.mIsActive = true;
				p.mPosition = mStartPos;
				p.mVelocity = direction_by_shape_type(mShapeType);
				p.mSpeed = Rng::generate_random_number(mMinSpeed, mMaxSpeed);
				p.mLifeTime = Rng::generate_random_number(mMinLifeTime, mMaxLifeTime);
				p.mColor = glm::vec4{
					Rng::generate_random_number(0.3f, 1.0f),
					Rng::generate_random_number(0.3f, 1.0f),
					Rng::generate_random_number(0.3f, 1.0f),
					1.0f
				};
				--num_to_emit;
			}
		}
	}

	mNumActive = 0;
	for (auto& p : mParticles)
	{
		if (p.mIsActive)
		{
			p.mLifeTime -= deltaTime;
			if (p.mLifeTime > 0.0f)
			{
				p.mPosition += p.mVelocity * deltaTime * p.mSpeed;
				++mNumActive;
			}
			else
			{
				p.mIsActive = false;
				p.mShouldExpire = true;
			}
		}
	}
	update_instance_buffer();

	if (mNumActive == 0 && mOnFinishCallback)
	{
		mOnFinishCallback();
	}
}

auto ParticleSystem::render(const RenderContext& ctx) const -> void
{
	if (!mIsEmitting || mNumActive == 0) { return; }

	ctx.mShader->bind();

	if (mTexture)
	{
		const auto texture_id = mTexture->get_id();
		glBindTexture(GL_TEXTURE_2D, texture_id);
	}

	const auto view = ctx.mView;
	const auto projection = ctx.mProjection;

	ctx.mShader->set_matrix("view", view);
	ctx.mShader->set_matrix("projection", projection);

	glBindVertexArray(mQuadVao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, mNumActive);
	glBindVertexArray(0);

	ctx.mShader->unbind();
}

auto ParticleSystem::generate_buffers() -> void
{
	mTexture = std::make_shared<Texture>(30, 30, Color{ .r = 255, .g = 0, .b = 0, .a = 30 });

	glGenVertexArrays(1, &mQuadVao);
	glGenBuffers(1, &mQuadVbo);
	glGenBuffers(1, &mInstanceVbo);
	glGenBuffers(1, &mColorsVbo);

	glBindVertexArray(mQuadVao);

	glBindBuffer(GL_ARRAY_BUFFER, mQuadVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleQuadVertices), ParticleQuadVertices, GL_STATIC_DRAW);

	utils::gl::UploadVertexAttribute(0, 2, 4 * sizeof(float), 0, nullptr);
	utils::gl::UploadVertexAttribute(1, 2, 4 * sizeof(float), 1, reinterpret_cast<void*>(2 * sizeof(float)));

	// Allocazione dei buffer di istanza e colore UNA VOLTA SOLA (alla massima capacità)
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVbo);
	glBufferData(GL_ARRAY_BUFFER, mMaxAmount * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
	for (auto i = 0u; i != 4; ++i)
	{
		constexpr auto vec4_size = sizeof(glm::vec4);
		glEnableVertexAttribArray(2 + i);
		glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(i * vec4_size));
		glVertexAttribDivisor(2 + i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mColorsVbo);
	glBufferData(GL_ARRAY_BUFFER, mMaxAmount * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

auto ParticleSystem::initialize_particles() -> void
{
	for (auto& [mPosition, mScale, mColor, mVelocity, mSpeed, mLifeTime, mRotation, mIsActive, mShouldExpire] : mParticles)
	{
		mPosition = mStartPos;
		mVelocity = direction_by_shape_type(mShapeType);
		mSpeed = Rng::generate_random_number(mMinSpeed, mMaxSpeed);
		mLifeTime = Rng::generate_random_number(mMinLifeTime, mMaxLifeTime);
		mColor = glm::vec4{
			Rng::generate_random_number(0.3f, 1.0f),
			Rng::generate_random_number(0.3f, 1.0f),
			Rng::generate_random_number(0.3f, 1.0f),
			1.0f
		};
		mIsActive = false;
		mShouldExpire = false;
	}
}

auto ParticleSystem::update_instance_buffer() const -> void
{
	auto model_matrices = std::vector<glm::mat4>{};
	auto colors = std::vector<glm::vec4>{};

	model_matrices.reserve(mNumActive);
	colors.reserve(mNumActive);

	for (const auto& p : mParticles)
	{
		if (!p.mIsActive) continue;

		const auto translation = glm::translate(glm::identity<glm::mat4>(), glm::vec3(p.mPosition, 0.0f));
		const auto rotation = glm::rotate(glm::identity<glm::mat4>(), p.mRotation, glm::vec3(0.0f, 0.0f, 1.0f));
		const auto scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3{ p.mScale, 1.0f });
		model_matrices.push_back(translation * rotation * scale);
		colors.push_back(p.mColor);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mInstanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_matrices.size() * sizeof(glm::mat4), model_matrices.data());

	glBindBuffer(GL_ARRAY_BUFFER, mColorsVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(glm::vec4), colors.data());
}