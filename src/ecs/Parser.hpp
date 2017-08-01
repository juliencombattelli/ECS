#ifndef ECS_PARSER_HPP_
#define ECS_PARSER_HPP_

#include <ecs/Entity.hpp>
#include <mul/stdx/tuple.hpp>
#include <string>

namespace ecs
{

template<typename... TComponents>
class Parser
{
public:

	template<typename TComponent>
	struct ComponentInfo
	{
		using type = TComponent;
		std::string name;
	};

	template<typename TComponent>
	void setName(const std::string& name);

	void parse(ecs::Entity::Id entityId, ecs::EntityManager<TComponents...>& em, const std::map<std::string,std::string>& data);

private:

	std::tuple<ComponentInfo<TComponents>...> m_componentsInfo;
};

template<typename... TComponents>
template<typename TComponent>
void Parser<TComponents...>::setName(const std::string& name)
{
	std::get<ComponentInfo<TComponent>>(m_componentsInfo).name = name;
}

template<typename... TComponents>
void Parser<TComponents...>::parse(ecs::Entity::Id entityId, ecs::EntityManager<TComponents...>& em, const std::map<std::string,std::string>& data)
{
	mul::for_each_in_tuple(m_componentsInfo, [entityId,&em](auto& tupleElement, const auto& datamap)
	{
		for(auto&& [name,value] : datamap)
		{
			if(name == tupleElement.name)
			{
				typename std::remove_reference<decltype(tupleElement)>::type::type c{};

				ecs::fromString(c,value);

				em.addComponent(entityId, std::forward<decltype(c)>(c));
			}
		}

	}, data);
}

} // namespace ecs

#endif /* ECS_PARSER_HPP_ */
