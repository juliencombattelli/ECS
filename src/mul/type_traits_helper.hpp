/*
 * type_traits
 *
 *  Created on: Jul 25, 2017
 *      Author: jucom
 */

#ifndef MUL_TYPE_TRAITS_HELPER_HPP_
#define MUL_TYPE_TRAITS_HELPER_HPP_

#include <type_traits>



/*
 * std::disjunction provider regardless of the standard version
 */
#if __cplusplus < 201703L

/*
 * Possible implementation from cppreference.com
 *
 * We can add it directly into std namespace since std::disjunction is provided by C++17 (__cplusplus >= 201703)
 */
namespace std {
template<class...> struct disjunction : std::false_type { };
template<class B1> struct disjunction<B1> : B1 { };
template<class B1, class... Bn>
struct disjunction<B1, Bn...>
    : std::conditional_t<bool(B1::value), B1, disjunction<Bn...>>  { };
}// namespace std

#endif // std::disjunction provider





/*
 * std::conjunction provider regardless of the standard version
 */
#if __cplusplus < 201703L

/*
 * Possible implementation from cppreference.com
 *
 * We can add it directly into std namespace since std::conjunction is provided by C++17 (__cplusplus >= 201703)
 */
namespace std {
template<class...> struct conjunction : std::true_type { };
template<class B1> struct conjunction<B1> : B1 { };
template<class B1, class... Bn>
struct conjunction<B1, Bn...>
    : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};
}// namespace std

#endif // std::conjunction provider





/*
 * Supplies mul::contains, which allows to check if a type T is present in a parameter pack Ts
 * This type T may be present multiple times in Ts
 */
namespace mul {

template<typename T, typename... Ts>
struct contains
{
	static constexpr bool value = std::disjunction<std::is_same<T, Ts>...>::value;
};

} // namespace mul




/*
 * Supplies mul::is_subset_of, which allows to check if a list of types is a subset of another
 * Types in Ts may be present multiple times in Us
 */
namespace mul {

template <typename Subset, typename Set>
struct is_subset_of
{
	static constexpr bool value = false;
};

template <typename... Ts, typename... Us>
struct is_subset_of<std::tuple<Ts...>, std::tuple<Us...>>
{
	static constexpr bool value = std::conjunction<mul::contains<Ts, Us...>...>::value;
};

} // namespace mul




/*
 * Supplies mul::tuple_get_helper, which allows to get the address of an element of a tuple by its type
 * If multiple elements of the same type are present in the tuple, this function raise a build error
 * If a type is not present in the tuple, this function return nullptr
 *
 * The first version (C++14 and below) use the too much verbose std::enable_if and SFINAE
 * The second one (C++17 and above) use the powerful if constexpr
 */
#if __cplusplus < 201703L
namespace mul {
template<typename T, typename... Ts>
typename std::enable_if< contains<T,Ts...>(), T* >::type tuple_get_helper(std::tuple<Ts...>& tpl)
{
	return &std::get<T>(tpl);
}

template<typename T, typename... Ts>
typename std::enable_if< !contains<T,Ts...>(), T* >::type tuple_get_helper(std::tuple<Ts...>&)
{
	return nullptr;
}
} // namespace mul
#else
namespace mul {
template<typename T, typename... Ts>
T* tuple_get_helper(std::tuple<Ts...>& tpl)
{
	if constexpr( contains<T,Ts...>() )
		return &std::get<T>(tpl);
	else
		return nullptr;
}
} // namespace mul
#endif


#endif /* MUL_TYPE_TRAITS_HELPER_HPP_ */
