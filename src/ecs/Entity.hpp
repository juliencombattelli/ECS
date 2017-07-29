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
 * Class representing an entity.
 * Internally, an entity is simply an integer id, and must be unique for a given entity.
 * Id of destroyed entities are not yet recycled.
 * Thus, the integer type of id must be long enough to represent the number of entity you need ( >= 32 bits is recommended, default is sizeof(std::size_t) )
 */
struct Entity
{
	using Id = std::size_t;

	static constexpr Id InvalidId{0};
};

/*
 * Class responsible of :
 * 		- the entities id assignment
 * 		- holding and retrieve the components associate to an entity
 */
template<typename... TComponents>
class EntityManager
{
	/*
	 * Define an alias to a container type
	 */
	template<typename T>
	using ComponentContainer = std::map<Entity::Id,T>;

public:

	/*
	 * Create an entity with uninitialized components
	 */
	template<typename... Ts>
	Entity::Id createEntity();

	/*
	 * Create an entity with initialized components (copy values from variable list of argument)
	 */
	template<typename... Ts>
	Entity::Id createEntity(Ts&&... components);

	/*
	 * Create an entity with initialized components (copy values from tuple)
	 */
	template<typename... Ts>
	Entity::Id createEntity_tuple(const std::tuple<Ts...>& components);

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

private:

	/*
	 * Counter used to generate entity id
	 */
	Entity::Id m_entityCount{Entity::InvalidId};

	/*
	 * Tuple of containers of all component types
	 */
	std::tuple<ComponentContainer<TComponents>...> m_components;
};

template<typename... TComponents>
template<typename... Ts>
Entity::Id EntityManager<TComponents...>::createEntity()
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value,"");

	Entity::Id id = ++m_entityCount;

	( ( std::get<ComponentContainer<Ts>>(m_components)[id] = Ts{} ) , ... );

	return id;
}

template<typename... TComponents>
template<typename... Ts>
Entity::Id EntityManager<TComponents...>::createEntity(Ts&&... components)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value,"");

	Entity::Id id = ++m_entityCount;

	( ( std::get<ComponentContainer<Ts>>(m_components)[id] = components ) , ... );

	return id;
}

template<typename... TComponents>
template<typename... Ts>
Entity::Id EntityManager<TComponents...>::createEntity_tuple(const std::tuple<Ts...>& components)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value,"");

	Entity::Id id = ++m_entityCount;

	( ( std::get<ComponentContainer<Ts>>(m_components)[id] = std::get<Ts>(components) ) , ... );

	/*std::apply( [id,this](auto&&... args) {
		( (std::get<ComponentContainer<Ts>>(m_components)[id] = args) , ... );
	} , components );*/

	return id;
}

template<typename... TComponents>
template<typename T>
inline T& EntityManager<TComponents...>::getComponent(Entity::Id entityId)
{
	static_assert(mul::contains<T,TComponents...>::value,"T must be in TComponents");

	ComponentContainer<T>& components = std::get<ComponentContainer<T>>(m_components);
	return components.at(entityId);
}

#endif

} // namespace ecs

#endif /* ECS_ENTITY_HPP_ */
