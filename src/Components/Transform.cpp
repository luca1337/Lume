#include <Lume/components/Transform.h>

Transform::Transform(const std::shared_ptr<Actor>& owner) : Component{ owner } {}

auto Transform::set_parent(const std::shared_ptr<Transform>& parent) -> void
{
	if (parent == mParent.lock()) { return; }

	if (const auto old_parent = mParent.lock())
	{
		old_parent->remove_child(std::static_pointer_cast<Transform>(shared_from_this()));
	}

	if (parent)
	{
		parent->add_child(std::static_pointer_cast<Transform>(shared_from_this()));
		mLocalPosition = mPosition - parent->mPosition;
		mPreviousParentRotation = parent->mRotation;
		mPreviousParentScale = parent->mScale;
	}

	mParent = parent;
}

auto Transform::set_position(const glm::vec2& pos) -> void
{
	mPosition = pos;

	if (const auto parent = mParent.lock())
	{
		mLocalPosition = mPosition - parent->mPosition;
	}

	for (const auto& child : mChildren)
	{
		child->update_child_position();
	}
}

auto Transform::set_local_position(const glm::vec2& pos) -> void
{
	mLocalPosition = pos;

	if (const auto parent = mParent.lock())
	{
		mPosition = parent->mPosition + mLocalPosition;
	}
	else
	{
		mPosition = mLocalPosition;
	}

	for (const auto& child : mChildren)
	{
		child->update_child_position();
	}
}

auto Transform::set_rotation(const float rot) -> void
{
	mRotation = rot;

	for (const auto& child : mChildren)
	{
		child->update_child_rotation();
	}
}

auto Transform::set_euler_degrees(const float deg) -> void
{
	set_rotation(glm::radians(deg));
}

auto Transform::set_scale(const glm::vec2& s) -> void
{
	mScale = s;

	for (const auto& child : mChildren)
	{
		child->update_child_scale();
	}
}

auto Transform::update_child_position() -> void
{
	if (const auto parent = mParent.lock())
	{
		mPosition = parent->mPosition + mLocalPosition;
	}
}

auto Transform::update_child_rotation() -> void
{
	if (const auto parent = mParent.lock())
	{
		const auto delta = parent->mRotation - mPreviousParentRotation;
		mRotation += delta;

		const float c = glm::cos(-delta);
		const float s = glm::sin(-delta);
		const glm::mat2 rot_matrix(c, -s, s, c);

		mLocalPosition = rot_matrix * mLocalPosition;
		mPosition = parent->mPosition + mLocalPosition;

		mPreviousParentRotation = parent->mRotation;
	}
}

auto Transform::update_child_scale() -> void
{
	if (const auto parent = mParent.lock())
	{
		const auto ratio = parent->mScale / mPreviousParentScale;
		mScale *= ratio;
		mPosition = parent->mPosition + mLocalPosition * ratio;
		mPreviousParentScale = parent->mScale;
	}
}

auto Transform::add_child(const std::shared_ptr<Transform>& child) -> void
{
	if (std::ranges::find(mChildren, child) == mChildren.end())
	{
		mChildren.emplace_back(child);
	}
}

auto Transform::remove_child(const std::shared_ptr<Transform>& child) -> void
{
	std::erase(mChildren, child);
}
