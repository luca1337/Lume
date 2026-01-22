#pragma once

#include <Lume/IRenderable.h>
#include <memory>
#include <Lume/components/Collider.h>

class Sprite;

class PixelCollider final : public Collider, public IRenderable
{
public:
	explicit PixelCollider(const std::shared_ptr<Actor>& owner, const std::shared_ptr<Sprite>& sprite);

	auto get_bounds() const->glm::vec4 override;
	auto collides_with(const Collider& other, glm::vec2& impactPoint) const -> bool override;

	// Inherited via irenderable
	auto render(const RenderContext& ctx) -> void override;
private:
	std::shared_ptr<Sprite> mSprite;
	std::shared_ptr<Sprite> mColliderSprite;
};
#pragma once
