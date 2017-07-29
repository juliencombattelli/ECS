//============================================================================
// Name        : entity.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <memory>
#include <vector>
#include <cassert>
#include <map>
#include <cstdint>

#include <mul/type_traits_helper.hpp>
#include <ecs/Entity.hpp>

struct Position
{
    float x,y;
};

struct Health
{
    int value;
};

struct Name
{
    std::string value;
};

struct Power
{
    int value;
};

struct Protection
{
    int value;
};
/*
using EntityManager = ecs::EntityManager<Health, Power, Protection>;
using Id = EntityManager::Id;


template <typename T, typename = int>
struct has_value : std::false_type { };

template <typename T>
struct has_value <T, decltype((void) T::value, 0)> : std::true_type { };

template<typename TComponent>
void print(TComponent&& c)
{
	if constexpr(std::is_reference<TComponent>::value)
		static_assert(has_value<typename std::remove_reference<TComponent>::type>::value, "TComponent must have a \"value\" member");
	else
		static_assert(has_value<TComponent>::value, "TComponent must have a \"value\" member");

	std::cout << "\t" << typeid(c).name() << " = " << c.value << std::endl;
}

struct SystemAttack
{
	void update(EntityManager& em, Id atk, const std::vector<Id>& defs)
	{
		Health& atk_hlt = em.get<Health>(atk);
		Power& atk_pow = em.get<Power>(atk);
		Protection& atk_pro = em.get<Protection>(atk);

		std::cout << "Atk #" << atk << " :" << std::endl;
		print(atk_hlt);
		print(atk_pow);
		print(atk_pro);

		for(auto def : defs)
		{
			Health& def_hlt = em.get<Health>(def);
			Power& def_pow = em.get<Power>(def);
			Protection& def_pro = em.get<Protection>(def);

			std::cout << "Def #" << def << " :" << std::endl;
			print(def_hlt);
			print(def_pow);
			print(def_pro);

			def_hlt.value -= (atk_pow.value > def_pro.value) ? (atk_pow.value - def_pro.value) : 0;
		}
	}
};

using PlayerComponent = std::tuple<Health, Power, Protection>;

int main()
{
    using namespace std;

    EntityManager mgr;

    Id e1 = mgr.createEntity<PlayerComponent>();
    Id e2 = mgr.createEntity<PlayerComponent>();

    mgr.initComponents(e1, Health{100}, Power{10}, Protection{5});
    mgr.initComponents(e2, Health{100}, Power{10}, Protection{5});

    SystemAttack atk;
    atk.update(mgr,e1,{e2});

    std::cout << mgr.get<Health>(e2).value << std::endl;

    return 0;
}*/

int main()
{
	std::tuple<Name,Health> tuple{{"me"},{10}};

	ecs::EntityManager<Health,Name> em;

	ecs::Entity::Id e1 = em.createEntity_tuple(tuple);

	std::cout << em.getComponent<Name>(e1).value << std::endl;

	return 0;
}
