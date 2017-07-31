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
#include <sstream>

#include <mul/type_traits_helper.hpp>
#include <mul/stdx/tuple.hpp>
#include <mul/stdx/index_range.hpp>
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
/*
std::map<std::string,std::string> data{
	{"name","me"},
	{"health","10"}
};
*/
template<typename T>
struct pair
{
	std::string name;
	using value_type = T;
};

std::tuple<pair<Name>,pair<Health>> t;


namespace test2
{
/*
 * thanks to Morwenn (https://codereview.stackexchange.com/questions/101137/template-integer-range-version-2)
 */
namespace detail
{
    template<
        typename Integer,
        typename SequenceBase,
        Integer Begin,
        Integer Step,
        bool IsIncreasing
    >
    struct integer_range_impl;

    template<
        typename Integer,
        Integer... N,
        Integer Begin,
        Integer Step
    >
    struct integer_range_impl<Integer, std::integer_sequence<Integer, N...>, Begin, Step, true>
    {
        using type = std::integer_sequence<Integer, (N * Step + Begin)...>;
    };

    template<
        typename Integer,
        Integer... N,
        Integer Begin,
        Integer Step
    >
    struct integer_range_impl<Integer, std::integer_sequence<Integer, N...>, Begin, Step, false>
    {
        using type = std::integer_sequence<Integer, (Begin - N * Step)...>;
    };
}

template<
    typename Integer,
    Integer Begin,
    Integer End,
    Integer Step
>
using make_integer_range = typename detail::integer_range_impl<
    Integer,
    std::make_integer_sequence<
        Integer,
        ((Begin < End ? End - Begin : Begin - End) - 1) / Step + 1
    >,
    Begin,
    Step,
    (Begin < End)
>::type;

template<std::size_t Begin, std::size_t End, std::size_t Step>
using make_index_range = make_integer_range<std::size_t, Begin, End, Step>;

}

template<typename Tuple, std::size_t... Ind>
void print(Tuple&& t, std::index_sequence<Ind...> d)
{
    ((std::cout << std::get<Ind>(t).name << ' ') , ...);
}

int main()
{
	std::get<pair<Name>>(t).name = "namesdcsdcsdcsdc";
	std::get<pair<Health>>(t).name = "health";

	//print_tuple_impl(t, 0 , ...,  12);

	print(t, test2::make_index_range<0u, 2u, 1u>{});

	/*for(auto i : mul::stdx::index_range(0,std::tuple_size<decltype(t)>::value))
	{
		auto& element = mul::stdx::get(t,i);
		for(const auto& pair : data)
		{
			if(pair.first == element.name)
			{
				std::stringstream ss(data[element.name]);
				typename std::remove_reference<decltype(element)>::type::value_type c;
				ss >> c.value;
				std::cout << typeid(decltype(c)).name() << " = " << c.value << std::endl;
			}
		}
	}*/

	/*auto f = [&data](auto t, int i)
	{
		if(data.begin()->first == std::get<i>(t).name)
		{
			typename std::remove_reference<decltype(std::get<i>(t))>::type::value_type c{std::stoi(data[std::get<i>(t).name])};
			std::cout << typeid(decltype(c)).name() << " = " << c.value << std::endl;
		}
	};

	detail::for_each(t, f, detail::gen_seq<1>());*/

	std::tuple<Name,Health> tuple{{"me"},{10}};

	ecs::EntityManager<Health,Name> em;

	ecs::Entity::Id e1 = em.createEntity_tuple(tuple);

	std::cout << em.getComponent<Name>(e1).value << std::endl;

	em.removeComponent<Name,Health>(e1);

	return 0;
}
