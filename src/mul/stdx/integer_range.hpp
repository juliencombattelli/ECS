#ifndef MUL_STDX_INTEGER_RANGE_HPP_
#define MUL_STDX_INTEGER_RANGE_HPP_

/*
 * This file provide an compile-time integer range generator.
 * These ranges are define by the integer type inside them, the start and end value, and a step
 *
 * Thanks to Morwenn (https://codereview.stackexchange.com/questions/101137/template-integer-range-version-2)
 */
namespace mul
{

/*
 * Some implementation details, please do not use
 */
namespace integer_range_detail
{
    template<typename TInt, typename SequenceBase, TInt Begin, TInt Step, bool IsIncreasing>
    struct integer_range_impl;

    template<typename TInt, TInt... N, TInt Begin, TInt Step>
    struct integer_range_impl<TInt, std::integer_sequence<TInt, N...>, Begin, Step, true>
    {
        using type = std::integer_sequence<TInt, (N * Step + Begin)...>;
    };

    template<typename TInt, TInt... N, TInt Begin, TInt Step>
    struct integer_range_impl<TInt, std::integer_sequence<TInt, N...>, Begin, Step, false>
    {
        using type = std::integer_sequence<TInt, (Begin - N * Step)...>;
    };

    template<typename TInt, TInt Begin, TInt End, TInt Step>
    using int_seq = std::make_integer_sequence<TInt,((Begin < End ? End - Begin : Begin - End) - 1) / Step + 1>;

} // namespace integer_range_detail

/*
 * make_integer_range
 * 		TInt : integer type hold by the range
 * 		Begin : start value of the range
 * 		End : end value of the range
 * 		Step : step value of the range
 *
 * 		values of the generated range are in the interval [Begin,End)
 */
template<typename TInt, TInt Begin, TInt End, TInt Step = (TInt)1>
using make_integer_range =
		typename integer_range_detail::integer_range_impl<TInt, integer_range_detail::int_seq<TInt,Begin,End,Step>, Begin, Step, (Begin < End)>::type;

/*
 * make_index_range
 * 		Specialization for unsigned integer (std::size_t)
 * 		Use this one if you want a range generator for array/tuple subscripting
 */
template<std::size_t Begin, std::size_t End, std::size_t Step = (std::size_t)1u>
using make_index_range =
		make_integer_range<std::size_t, Begin, End, Step>;

} // namespace mul

#endif /* MUL_STDX_INTEGER_RANGE_HPP_ */
