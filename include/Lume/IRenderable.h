#pragma once

struct RenderContext;

class IRenderable
{
public:
	virtual auto render(const RenderContext& ctx) -> void = 0;
	virtual ~IRenderable() = default;
};
