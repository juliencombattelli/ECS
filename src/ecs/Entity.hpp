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

	template<typename T>
	T& get(Id entityId)
	{
		static_assert(mul::contains<T,TComponents...>::value,"T must be in TComponents");
		ComponentContainer<T>& components = std::get<ComponentContainer<T>>(m_components);
		return components.at(entityId);
	}

private:

	static constexpr Id InvalidId{0};
	Id m_entityCount{InvalidId+1};
	std::tuple<ComponentContainer<TComponents>...> m_components;
};

} // namespace ecs

#endif /* ECS_ENTITY_HPP_ */
