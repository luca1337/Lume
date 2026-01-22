#include <Lume/Actor.h>
#include <Lume/ResourceManager.h>
#include <Lume/Sprite.h>
#include <Lume/World.h>
#include <Lume/components/SpriteRenderer.h>
#include <Lume/components/Transform.h>
#include <Lume/rendering/Camera.h>
#include <Lume/rendering/RenderContext.h>
#include <Lume/rendering/Shader.h>

SpriteRenderer::SpriteRenderer(const std::shared_ptr<Actor>& owner, const std::shared_ptr<Sprite>& sprite) : Component{ owner }, mSprite{ sprite }
{
	const auto& cam = owner->get_world().get_camera();
	const auto& sprite_shader = ResourceManager::get_from_cache<Shader>({ .mResourceType = ResourceType::Shader, .mName = "SpriteShader" });
	render_context() = {.mView = cam->get_view_matrix(), .mProjection = cam->get_projection_matrix(), .mShader = sprite_shader.value()};
}

void SpriteRenderer::update(float deltaTime)
{
}

void SpriteRenderer::render(const RenderContext& ctx)
{
	if (!mSprite) { return; }

	if (const auto& ow = owner().lock(); ow)
	{
		const auto& transform = ow->transform();
		mSprite->render(ctx, *transform);
	}
}
