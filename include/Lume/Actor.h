#pragma once

#include <concepts>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

class World;
class Transform;
class Component;

template <typename T>
concept IsComponent = std::derived_from<T, Component>;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
	virtual ~Actor() = default;
	explicit Actor(World& w);
	explicit Actor(World& w, std::string name);

	virtual auto awake() -> void {}
	virtual auto start() -> void {}
	virtual auto update(float deltaTime) -> void;
	virtual auto fixed_update(float fixedDeltaTime) -> void;

	template <IsComponent T, typename... Args>
	auto add_component(Args&&... args) -> std::shared_ptr<T>
	{
		if (auto existing = get_component<T>())
		{
			return existing;
		}

		auto self = shared_from_this();
		auto new_comp = std::make_shared<T>(self, std::forward<Args>(args)...);
		m_components[std::type_index(typeid(T))] = new_comp;
		return new_comp;
	}

	template <IsComponent T>
	auto get_exact_component() const -> std::shared_ptr<T>
	{
		if (const auto it = m_components.find(std::type_index(typeid(T))); it != m_components.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return {};
	}

	template <IsComponent T>
	auto get_component() const -> std::shared_ptr<T>
	{
		for (const auto& comp : m_components | std::views::values)
		{
			if (auto casted = std::dynamic_pointer_cast<T>(comp))
			{
				return casted;
			}
		}
		return {};
	}

	template <IsComponent T>
	auto has_component() const { return get_component<T>() != nullptr; }

	[[nodiscard]] auto& enabled() { return m_enabled; }
	[[nodiscard]] const auto& enabled() const { return m_enabled; }

	[[nodiscard]] auto& transform() { return m_transform; }
	[[nodiscard]] const auto& transform() const { return m_transform; }

	[[nodiscard]] const auto& get_name() const { return m_name; }

	[[nodiscard]] const auto& get_world() const { return m_world; }
	[[nodiscard]] auto& get_world() { return m_world; }

private:
	World& m_world;
	std::string m_name = "default";
	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
	std::shared_ptr<Transform> m_transform = {};
	bool m_enabled = true;
	bool m_is_started = {};

	friend class World;

	auto configure_defaults() -> void;
};
