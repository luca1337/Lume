#include <Lume/Actor.h>
#include <Lume/Collisions/CollisionSystem.h>
#include <Lume/Enums.h>
#include <Lume/ResourceManager.h>
#include <Lume/Window.h>
#include <Lume/World.h>
#include <Lume/rendering/Shader.h>

#include "Lume/rendering/RenderContext.h"
#include "Lume/Rendering/Debugs/DebugDrawer.h"

auto World::setup() -> void
{
	// -- Register Resources
	const auto default_shader = std::make_shared<Shader>("resources/shaders/vertex_sprite.glsl", "resources/shaders/fragment_sprite.glsl");
	ResourceManager::register_resource<Shader>(ResourceParams{ .mResourceType = ResourceType::Shader, .mName = "SpriteShader", .mResource = default_shader });

	const auto particle_shader = std::make_shared<Shader>("resources/shaders/vertex_particle.glsl", "resources/shaders/fragment_particle.glsl");
	ResourceManager::register_resource<Shader>(ResourceParams{ .mResourceType = ResourceType::Shader, .mName = "ParticleShader", .mResource = particle_shader });

	const auto line_shader = std::make_shared<Shader>("resources/shaders/vertex_line.glsl", "resources/shaders/fragment_line.glsl");
	ResourceManager::register_resource<Shader>(ResourceParams{ .mResourceType = ResourceType::Shader, .mName = "LineShader", .mResource = line_shader });
}

auto World::update() const -> void
{
	mWindow->update([&](const float deltaTime) {
		for (const auto& actor : mActors)
		{
			if (actor->enabled())
			{
				actor->update(deltaTime);
			}
		}

		collision_system::update_collisions(*this);

		debug_drawer::render(*get_camera());

		}, [&](const float fixedDeltaTime) {
			for (const auto& actor : mActors)
			{
				if (actor->enabled())
				{
					actor->fixed_update(fixedDeltaTime);
				}
			}});
}