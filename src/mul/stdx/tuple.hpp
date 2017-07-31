#ifndef MUL_STDX_TUPLE_HPP_
#define MUL_STDX_TUPLE_HPP_

namespace mul { namespace stdx
{

namespace detail
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

} // namespace detail



/*
 * Runtime replacement for std::get<...>(tuple)
 * Return a reference to the index-th element
 */
template<typename TTuple> constexpr
typename std::tuple_element<0,detail::tuple_type<TTuple>>::type& get(TTuple&& t,size_t index)
{
    using tuple_type=typename std::remove_reference<TTuple>::type;

    if(index>=std::tuple_size<tuple_type>::value)
        throw std::runtime_error("Out of range");

    return detail::tuple_get_func_table<tuple_type>::table[index](t);
}

}} // namespace mul::stdx

#endif /* MUL_STDX_TUPLE_HPP_ */
