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

#include <json/json.hpp>

#include <mul/type_traits_helper.hpp>
#include <mul/stdx/tuple.hpp>
#include <mul/stdx/index_range.hpp>
#include <mul/stdx/integer_range.hpp>
#include <ecs/Entity.hpp>
#include <ecs/Parser.hpp>

#include <muParser.h>
#include <cxxabi.h>

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

using json = nlohmann::json;

json j2 =
{
  {"name", "hello"},
  {"health", 25},
  {"position", {0.1,0.2}}
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

template<>
void ecs::fromString<Position>(Position& pos, const std::string& str)
{
	std::string::size_type separator = str.find(',');

	pos.x = stof(str.substr(0, separator));
	pos.y = stof(str.substr(separator+1));
}


template<typename TVal, typename TRes>
void round(TVal value, TRes& result)
{
    static_assert(std::is_floating_point<TVal>::value || std::is_integral<TVal>::value, "TVal must be a floating point or an integral value");

    if constexpr(std::is_floating_point<TRes>::value)
    {
        result = std::round(value);
    }
    else if constexpr(std::is_same<TRes,long long>::value || std::is_same<TRes,unsigned long long>::value)
    {
        result = std::llround(value);
    }
    else if constexpr(std::is_integral<TRes>::value)
    {
        result = std::lround(value);
    }
}

template<typename... Ts>
class Parser
{
public:

    using value_type = mu::value_type;

    Parser()
    {
        // We use dot notation in formulae (e.g.: object.member), so add it to charset
        m_parser.DefineNameChars(_T("0123456789_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ."));
    }

    void defineConst(const std::string& name, value_type value)
    {
        m_parser.DefineConst(name, value);
    }

    template<typename T>
    void defineVar(const std::string& name, T& var)
    {
        value_type* ptr = &std::get<std::map<T*,value_type>>(m_vars)[&var];
        *ptr = static_cast<value_type>(var);
        m_parser.DefineVar(name, ptr);
    }

    value_type eval(const std::string& formula)
    {
        value_type retval = 0;

        try
        {
            m_parser.SetExpr(formula);
            retval = m_parser.Eval();

            mul::for_each_in_tuple(m_vars, [](auto& varmap)
            {
                for(auto&& [varptr,value] : varmap)
                    round(value, *varptr);
            });
        }
        catch(mu::ParserError& e)
        {
            std::cout << e.GetMsg() << _T(" (Errc: ") << std::dec << e.GetCode() << _T(")") << std::endl;
        }

        return retval;
    }

private:

    mu::Parser m_parser;
    std::tuple<std::map<Ts*,value_type>...> m_vars;
};





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
	void update(ecs::EntityManager<Health,Name,Position,Protection,Power>& em, ecs::Entity::Id atk, const std::vector<ecs::Entity::Id>& defs)
	{
		Health& atk_hlt = em.getComponent<Health>(atk);
		Power& atk_pow = em.getComponent<Power>(atk);
		Protection& atk_pro = em.getComponent<Protection>(atk);

		/*std::cout << "Atk #" << atk << " :" << std::endl;
		print(atk_hlt);
		print(atk_pow);
		print(atk_pro);*/

		for(auto def : defs)
		{
			Health& def_hlt = em.getComponent<Health>(def);
			Power& def_pow = em.getComponent<Power>(def);
			Protection& def_pro = em.getComponent<Protection>(def);

			/*std::cout << "Def #" << def << " :" << std::endl;
			print(def_hlt);
			print(def_pow);
			print(def_pro);*/

			parser.defineConst("atk.pow", atk_pow.value);
			parser.defineConst("def.pro", def_pro.value);
			parser.defineVar("def.hlt", def_hlt.value);

			parser.eval("def.hlt = def.hlt -(atk.pow - def.pro)");
		}
	}

	Parser<int,float> parser;
};


/*
 *  Technique file descriptor
 */
/*
{
	name : "Burning blade",
	element : fire,
	hp_damage_formula : "(atk.pow + atk.mag/2) - (def.pro + def.prom/3)",
	status : [ {burn,0.6} ],
	animation : "burning_blade.anim"

}
*/


int main()
{
	std::map<std::string,std::string> data1
	{
		{"name","e1"},
		{"health","10"},
		{"position","2.1,3.2"},
		{"power","3"},
		{"protection","1"}
	};

	std::map<std::string,std::string> data2
	{
		{"name","e2"},
		{"health","10"},
		{"position","2.1,3.2"},
		{"power","3"},
		{"protection","1"}
	};

	ecs::EntityManager<Health,Name,Position,Protection,Power> em;
	ecs::Parser<Health,Name,Position,Protection,Power> parser;

	parser.setName<Health>("health");
	parser.setName<Name>("name");
	parser.setName<Position>("position");
	parser.setName<Protection>("protection");
	parser.setName<Power>("power");


	ecs::Entity::Id e1 = em.createEntity();
	ecs::Entity::Id e2 = em.createEntity();

	parser.parse(e1, em, data1);
	parser.parse(e2, em, data2);

	std::cout << em.getComponent<Name>(e1).value << std::endl;
	std::cout << em.getComponent<Health>(e1).value << std::endl;
	std::cout << em.getComponent<Position>(e1).x << " , " << em.getComponent<Position>(e1).y << std::endl;

	SystemAttack sysatk;
	sysatk.update(em, e1, {e2});

	std::cout << em.getComponent<Name>(e1).value << std::endl;
	std::cout << em.getComponent<Health>(e1).value << std::endl;

	std::cout << em.getComponent<Name>(e2).value << std::endl;
	std::cout << em.getComponent<Health>(e2).value << std::endl;

	//em.removeComponent<Name,Health>(e1);

	std::cout << j2.dump(4) << std::endl << std::endl;

	for (auto& it : j2)
		std::cout << json::iterator(&it).key() << std::endl;
		//std::cout << abi::__cxa_demangle(typeid(it).name(), nullptr, nullptr, nullptr) << std::endl;

	for (json::iterator it = j2.begin() ; it != j2.end() ; ++it)
		std::cout << abi::__cxa_demangle(typeid(it).name(), nullptr, nullptr, nullptr) << std::endl;

	return 0;
}
