#pragma once

class World;

namespace collision_system
{
	auto update_collisions(const World& w) -> void;
}
