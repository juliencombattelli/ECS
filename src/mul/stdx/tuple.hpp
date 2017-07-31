#ifndef MUL_STDX_TUPLE_HPP_
#define MUL_STDX_TUPLE_HPP_

#include <mul/stdx/integer_range.hpp>
#include <functional>

namespace mul
{

/*
 * Some implementation details, please do not use
 */
namespace tuple_get_detail
{

template<typename TTuple>
using tuple_type = typename std::remove_reference<TTuple>::type;

template<typename TTuple, typename TIndices = std::make_index_sequence<std::tuple_size<TTuple>::value>>
struct tuple_get_func_table;

template<typename TTuple, std::size_t... TIndices>
struct tuple_get_func_table<TTuple, std::index_sequence<TIndices...>>
{
    using return_type = typename std::tuple_element<0,TTuple>::type&;

    using get_func_ptr = return_type (*)(TTuple&) noexcept;

    static constexpr get_func_ptr table[std::tuple_size<TTuple>::value] =
    {
        &std::get<TIndices>...
    };
};

template<typename TTuple, size_t... TIndices>
constexpr typename tuple_get_func_table<TTuple,std::index_sequence<TIndices...>>::get_func_ptr
tuple_get_func_table<TTuple,std::index_sequence<TIndices...>>::table[std::tuple_size<TTuple>::value];

} // namespace tuple_get_detail

/* tuple_element& get(TupleType&& tuple, std::size_t index)
 * 		Runtime replacement for std::get<...>(tuple)
 * 		Return a reference to the index-th element
 */
template<typename TTuple> constexpr
typename std::tuple_element<0,tuple_get_detail::tuple_type<TTuple>>::type& get(TTuple&& t, std::size_t index)
{
    using tuple_type=typename std::remove_reference<TTuple>::type;

    if(index>=std::tuple_size<tuple_type>::value)
        throw std::runtime_error("Out of range");

    return tuple_get_detail::tuple_get_func_table<tuple_type>::table[index](t);
}

/*
 * for_each
 * 		Apply a functor to elements of tuple specified by an integer sequence
 *		Parameter for the functor are forwarded, and all return values are stored in the returned tuple
 */
template<typename TTuple, typename TFunc, std::size_t... Is, typename... Args>
void for_each(TTuple&& tuple, std::index_sequence<Is...>, TFunc&& func, Args&&... args)
{
	( func(std::get<Is>(tuple), std::forward<Args>(args)...) , ... );
}

/*
 * for_each_in_tuple
 * 		Apply a functor to all elements of tuple
 *		Parameter for the functor are forwarded, and all return values are stored in the returned tuple
 */
template<typename... Ts, typename TFunc, typename... Args>
void for_each_in_tuple(const std::tuple<Ts...>& tuple, TFunc&& func, Args&&... args)
{
    for_each(tuple, mul::make_index_range<0,sizeof...(Ts)>{}, func, std::forward<Args>(args)...);
}

} // namespace mul

#endif /* MUL_STDX_TUPLE_HPP_ */
