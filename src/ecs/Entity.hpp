/*
 * Entity.hpp
 *
 *  Created on: Jul 25, 2017
 *      Author: jucom
 */

#ifndef ECS_ENTITY_HPP_
#define ECS_ENTITY_HPP_

#include <ecs/Config.hpp>
#include <mul/type_traits_helper.hpp>
#include <string>

namespace ecs
{

/*
 * fromString(TComponent& comp, const std::string& str)
 * 		This function allowing the conversion of a std::string into a component
 * 		The component "comp" must fulfill the "Component" concept :
 * 			- a component must have a attribute "value"
 * 			- type of the value attribute must be streamable via << and >> operator
 * 		If your component does not fulfill these requirements, you can specialize fromString
 */
template<typename TComponent>
void fromString(TComponent& comp, const std::string& str)
{
	std::istringstream(str) >> comp.value;
}
/*
 * Struct Entity
 * 		Class representing an entity.
 * 		Internally, an entity is simply an integer id, and must be unique for a given entity.
 * 		Id of destroyed entities are not yet recycled.
 * 		Thus, the integer type of id must be long enough to represent the number of entity you need
 * 			( >= 32 bits is recommended, default is sizeof(std::size_t) )
 */
struct Entity
{
	using Id = std::size_t;

	static constexpr Id InvalidId{0};
};

/*
 * class EntityManager
 * 		Class responsible of :
 * 			- the entities id assignment
 * 			- holding and retrieve the components associate to an entity
 * 			- parse a map of key/value string and construct component following it (TODO: create Parser class for handle that)
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

	/*
	 *
	 */
	template<typename TTuple>
	void parse(Entity::Id entityId, TTuple&& componentMatchingTable, const std::map<std::string,std::string>& data)
	{
		mul::for_each_in_tuple(componentMatchingTable, [this,entityId](auto& tupleElement, const auto& datamap)
		{
			for(auto&& [name,value] : datamap)
			{
				if(name == tupleElement.name)
				{
					typename std::remove_reference<decltype(tupleElement)>::type::value_type c{};

					fromString(c,value);

					this->addComponent(entityId, std::forward<decltype(c)>(c));
				}
			}

		}, data);
	}

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

#define ECS_Ts_must_be_subset_of_T  "Types in Ts must be in TComponents"
#define ECS_T_must_be_included_in_T "Type T must be included in TComponents"

template<typename... TComponents>
template<typename... Ts>
Entity::Id EntityManager<TComponents...>::createEntity()
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value, ECS_Ts_must_be_subset_of_T);

	Entity::Id id = ++m_entityCount;

	addComponent<Ts...>(id);

	return id;
}

template<typename... TComponents>
template<typename... Ts>
Entity::Id EntityManager<TComponents...>::createEntity(Ts&&... components)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value, ECS_Ts_must_be_subset_of_T);

	Entity::Id id = ++m_entityCount;

	addComponent(id, std::forward<decltype(components)>(components)...);

	return id;
}

template<typename... TComponents>
template<typename... Ts>
Entity::Id EntityManager<TComponents...>::createEntity_tuple(const std::tuple<Ts...>& components)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value, ECS_Ts_must_be_subset_of_T);

	Entity::Id id = ++m_entityCount;

	addComponent_tuple(id, components);

	return id;
}

template<typename... TComponents>
template<typename... Ts>
void EntityManager<TComponents...>::addComponent(Entity::Id id)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value, ECS_Ts_must_be_subset_of_T);

	( ( std::get<ComponentContainer<Ts>>(m_components)[id] = Ts{} ) , ... );
}

template<typename... TComponents>
template<typename... Ts>
void EntityManager<TComponents...>::addComponent(Entity::Id id, Ts&&... components)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value, ECS_Ts_must_be_subset_of_T);

	( ( std::get<ComponentContainer<Ts>>(m_components)[id] = components ) , ... );
}

template<typename... TComponents>
template<typename... Ts>
void EntityManager<TComponents...>::addComponent_tuple(Entity::Id id, const std::tuple<Ts...>& components)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value, ECS_Ts_must_be_subset_of_T);

	( ( std::get<ComponentContainer<Ts>>(m_components)[id] = std::get<Ts>(components) ) , ... );
}

template<typename... TComponents>
template<typename T>
inline T& EntityManager<TComponents...>::getComponent(Entity::Id entityId)
{
	static_assert(mul::contains<T,TComponents...>::value, ECS_T_must_be_included_in_T);

	ComponentContainer<T>& components = std::get<ComponentContainer<T>>(m_components);
	return components.at(entityId);
}

template<typename... TComponents>
template<typename... Ts>
inline void EntityManager<TComponents...>::removeComponent(Entity::Id id)
{
	static_assert(mul::is_subset_of<std::tuple<Ts...>, std::tuple<TComponents...>>::value, ECS_Ts_must_be_subset_of_T);

	( ( std::get<ComponentContainer<Ts>>(m_components).erase(id) ) , ... );
}

} // namespace ecs

#endif /* ECS_ENTITY_HPP_ */
