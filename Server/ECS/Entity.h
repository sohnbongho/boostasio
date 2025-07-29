// Entity.h
#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <type_traits>
#include "IComponent.h"
#include "ISystem.h"
#include "IDisposable.h"


class Entity : public std::enable_shared_from_this<Entity>
{
public:
	Entity() = default;
	virtual ~Entity()
	{
		Dispose();
	}

	template<typename T>
	void AddComponent(std::shared_ptr<T> component) 
	{
		static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent");
		_components[typeid(T)] = std::move(component);
	}

	template<typename T>
	std::shared_ptr<T> GetComponent() 
	{
		auto it = _components.find(typeid(T));
		if (it != _components.end()) {
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return nullptr;
	}

	template<typename T>
	void AddSystem(std::shared_ptr<T> system) 
	{
		static_assert(std::is_base_of<ISystem, T>::value, "T must inherit from ISystem");
		_systems[typeid(T)] = std::move(system);
	}

	template<typename T>
	std::shared_ptr<T> GetSystem() {
		auto it = _systems.find(typeid(T));
		if (it != _systems.end()) {
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return nullptr;
	}

	void Dispose()
	{
		for (auto& it : _components)
		{
			auto component = it.second;

			// IDisposable로 캐스팅 시도
			if (auto disposable = std::dynamic_pointer_cast<IDisposable>(component))
			{
				disposable->Dispose();
			}
		}

		for (auto& it : _systems)
		{
			auto system = it.second;

			// IDisposable로 캐스팅 시도
			if (auto disposable = std::dynamic_pointer_cast<IDisposable>(system))
			{
				disposable->Dispose();
			}
		}

		_components.clear();
		_systems.clear();
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<IComponent>> _components;
	std::unordered_map<std::type_index, std::shared_ptr<ISystem>> _systems;
};
