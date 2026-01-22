#pragma once

struct HitInfo;

class CollisionListener
{
public:
	virtual ~CollisionListener() = default;

	virtual void on_collision_enter(const HitInfo& hit) {}
	virtual void on_collision_stay(const HitInfo& hit) {}
	virtual void on_collision_exit(const HitInfo& hit) {}
};