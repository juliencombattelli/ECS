/*
 * Entity.hpp
 *
 *  Created on: Jul 25, 2017
 *      Author: jucom
 */

#ifndef ECS_ENTITY_HPP_
#define ECS_ENTITY_HPP_

#include <mul/type_traits_helper.hpp>

namespace ecs {

#if 0

template<typename... TComponents>
class EntityManager
{
public:

	using Id = std::size_t;

	template<typename T>
	using ComponentContainer = std::map<Id,T>;

	template<typename... Ts>
	void initComponents(Id id, Ts&&... arg)
	{
		( ( std::get<ComponentContainer<Ts>>(m_components)[id] = arg ) , ... );
	}

	template<typename TComponentList>
	Id createEntity()
	{
		static_assert(mul::is_subset_of<TComponentList, std::tuple<TComponents...>>::value,"");

		std::apply( [this](auto&&... args) {
			initComponents(m_entityCount, std::forward<decltype(args)>(args)...);
		} , TComponentList{} );

		return m_entityCount++;
	}

	/*
	 * Get a component associate to an entity
	 * 	   If the component type is not handled by the manager, a static_assert fails
	 *     If the entity does not have this type of component, a runtime exception is raised
	 */
	template<typename T>
	T& get(Id entityId)
	{
		static_assert(mul::contains<T,TComponents...>::value,"T must be in TComponents");

		ComponentContainer<T>& components = std::get<ComponentContainer<T>>(m_components);
		return components.at(entityId);
	}

	/*
	 * Add a component to an entity
	 * 	   If the component type is not handled by the manager, a static_assert fails
	 *     If the entity already has a component of this type, the value is overwritten
	 */
	template<typename T>
	void add(Id entityId, T&& value)
	{
		static_assert(mul::contains<T,TComponents...>::value,"T must be in TComponents");

		ComponentContainer<T>& components = std::get<ComponentContainer<T>>(m_components);
		components[entityId] = value;
	}

	/*
	 * Remove a component associate to an entity
	 * 	   If the component type is not handled by the manager, a static_assert fails
	 *     If the entity already has a component of this type, the value is overwritten
	 */
	template<typename T>
	void remove(Id entityId)
	{
		static_assert(mul::contains<T,TComponents...>::value,"T must be in TComponents");

		ComponentContainer<T>& components = std::get<ComponentContainer<T>>(m_components);
		components.erase(entityId);
	}

private:

	static constexpr Id InvalidId{0};
	Id m_entityCount{InvalidId+1};
	std::tuple<ComponentContainer<TComponents>...> m_components;
};

#else

/*
 * Class representing an entity
 * Internally, an entity is simply an integer id, and must be unique for a given entity
 */
class Entity
{
	using Id = std::size_t;
};

/*
 * Class responsible of :
 * 		- the entities id assignment
 * 		- holding and retrieve the components associate to an entity
 */
template<typename... TComponents>
class EntityManager
{
public:

	/*
	 * Add components (uninitialized)
	 * 	   If the components type are not handled by the manager, a static_assert fails
	 */
	template<typename... Ts>
	void addComponent(Entity::Id id);

	/*
	 * Add components (initialized, copy the value from variable list of argument)
	 * 	   If the components type are not handled by the manager, a static_assert fails
	 */
	template<typename... Ts>
	void addComponent(Entity::Id id, Ts&&... components);

	/*
	 * Add components (initialized, copy the values from tuple)
	 * 	   If the components type are not handled by the manager, a static_assert fails
	 */
	template<typename... Ts>
	void addComponent_tuple(Entity::Id id, const std::tuple<Ts...>& components);

	/*
	 * Remove components
	 */
	template<typename... Ts>
	void removeComponent(Entity::Id entityId);

	/*
	 * Get a reference to a component associate to an entity
	 * 	   If the component type is not handled by the manager, a static_assert fails
	 *     If the entity does not have this type of component, a runtime exception is raised
	 */
	template<typename T>
	T& getComponent(Entity::Id entityId);
};

#endif

} // namespace ecs

#endif /* ECS_ENTITY_HPP_ */
