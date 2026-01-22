#pragma once

#include <Lume/components/Collider.h>

class Sprite;

class BoxCollider final : public Collider
{
public:
	explicit BoxCollider(const std::shared_ptr<Actor>& owner);

	auto get_bounds() const->glm::vec4 override;
	auto collides_with(const Collider& other, glm::vec2& impactPoint) const -> bool override;

private:
	std::shared_ptr<Sprite> mSprite;
};
