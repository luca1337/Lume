#pragma once

#include <memory>
#include <vector>

class Camera;
class Window;
class Actor;

class World final
{
public:
	~World() = default;
	World() = default;
	World(const World&) = delete;
	World(World&&) = delete;
	World& operator=(const World&) = delete;
	World& operator=(World&&) = delete;

	World(const std::shared_ptr<Window>& win, const std::shared_ptr<Camera>& cam) : mWindow{ win }, mCamera{ cam } {}
	static auto setup() -> void;
	auto update() const -> void;

	template <typename T, typename... Args>
	auto spawn(Args&& ... args) -> std::shared_ptr<T>
	{
		static_assert(std::is_base_of_v<Actor, T>);
		auto actor = std::make_shared<T>(*this, std::forward<Args>(args)...);
		actor->configure_defaults();
		actor->start();
		mActors.emplace_back(actor);
		return actor;
	}

	[[nodiscard]] const auto& get_actors() const { return mActors; }

	[[nodiscard]] const auto& get_window() const { return mWindow; }
	[[nodiscard]] const auto& get_camera() const { return mCamera; }

private:
	std::shared_ptr<Window> mWindow = {};
	std::shared_ptr<Camera> mCamera = {};
	std::vector<std::shared_ptr<Actor>> mActors = {};
};
