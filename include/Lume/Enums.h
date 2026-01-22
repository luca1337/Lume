#pragma once

#include <Lume/Export.h>

enum class RENDER_API ResourceType
{
	Shader,
	Texture,
	Audio,
	RenderCtx
};

enum class RENDER_API DirectionType
{
	Left,
	Right,
	Down
};

enum class RENDER_API ShapeType
{
	None,
	Circle,
	HalfCircle,
};