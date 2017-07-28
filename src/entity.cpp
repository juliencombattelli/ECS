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
    char* value;
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

template<typename... Ts>
void addComponent(int id)
{
	std::cout << "Entity #" << id << " from addComponent(int id)" << std::endl;

	std::apply( [](auto&&... args) {
				print(std::forward<decltype(args)>(args)...);
			} , std::tuple<Ts...>{} );
}

template<typename... Ts>
void addComponent(int id, Ts&&... components)
{
	std::cout << "Entity #" << id << " from addComponent(int id, Ts&&... components)" << std::endl;

	print(std::forward<decltype(components)>(components)...);
}


template<typename... Ts>
void addComponent_tuple(int id, const std::tuple<Ts...>& components)
{
	std::cout << "Entity #" << id << " from addComponent_t(int id, const std::tuple<Ts...>& components)" << std::endl;

	std::apply( [](auto&&... args) {
		print(std::forward<decltype(args)>(args)...);
	} , components);
}

int main()
{
	std::tuple<Health> tuple{Health{10}};

	addComponent<Health>(0);
	addComponent(0, Health{20});
	//addComponent(0, tuple); // static assert failed : tuple has no "value" member

	//addComponent_t<Health>(0); // build failed no matching function to call
	//addComponent_t(0, Health{20}); // build failed no matching function to call
	addComponent_tuple(0, tuple);

	return 0;
}
