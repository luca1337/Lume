#pragma once

#include <Lume/components/Component.h>
#include <Lume/IRenderable.h>
#include <memory>

class Sprite;

class SpriteRenderer final : public Component, public IRenderable
{
public:
	explicit SpriteRenderer(const std::shared_ptr<Actor>& owner, const std::shared_ptr<Sprite>& sprite);

	void update(float deltaTime) override;
	auto render(const RenderContext& ctx) -> void override;

	[[nodiscard]] const auto& sprite_raw() const { return mSprite; }
	[[nodiscard]] auto& sprite_raw() { return mSprite; }

private:
	std::shared_ptr<Sprite> mSprite;
};
