#include <Lume/Actor.h>
#include <Lume/IRenderable.h>
#include <ranges>
#include <Lume/components/Transform.h>

Actor::Actor(World& w) : m_world{ w } {}

Actor::Actor(World& w, std::string name) : m_world{ w }, m_name{ std::move(name) } { }

auto Actor::update(const float deltaTime) -> void
{
	for (const auto& comp : m_components | std::views::values)
	{
		if (!comp->enabled())
		{
			continue;
		}

		comp->update(deltaTime);

		// Render the component if it implements IRenderable such as SpriteRenderer, AnimationRenderer, etc.
		if (const auto& renderable_component = std::dynamic_pointer_cast<IRenderable>(comp))
		{
			if (auto& render_context = comp->render_context())
			{
				render_context->mEnableDebug = comp->debug();
				renderable_component->render(render_context.value());
			}
		}
	}
}

auto Actor::fixed_update(const float fixedDeltaTime) -> void
{
	for (const auto& comp : m_components | std::views::values)
	{
		if (!comp->enabled())
		{
			continue;
		}

		comp->fixed_update(fixedDeltaTime);

		// Render the component if it implements IRenderable such as SpriteRenderer, AnimationRenderer, etc.
		if (const auto& renderable_component = std::dynamic_pointer_cast<IRenderable>(comp))
		{
			if (auto& render_context = comp->render_context())
			{
				render_context->mEnableDebug = comp->debug();
				renderable_component->render(render_context.value());
			}
		}
	}
}

auto Actor::configure_defaults() -> void
{
	if (!m_transform)
	{
		m_transform = add_component<Transform>();
	}
}
