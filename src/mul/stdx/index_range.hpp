#ifndef MUL_STDX_INDEX_RANGE_HPP_
#define MUL_STDX_INDEX_RANGE_HPP_

#include <vector>
#include <cmath>

namespace mul { namespace stdx
{

class index_range
{
public:

	using index_type = std::size_t;
	using value_type = index_type;
	using size_type = std::vector<index_type>::size_type;
	using iterator = std::vector<index_type>::iterator;
	using const_iterator = std::vector<index_type>::const_iterator;
	using reference = std::vector<index_type>::reference;
	using const_reference = std::vector<index_type>::const_reference;

	index_range(index_type bgn, index_type end)
	{
		m_indeces.reserve(std::abs(static_cast<long long int>(end - bgn)));
		if(bgn <= end)
			for(index_type i = bgn ; i < end ; i++)
				m_indeces.push_back(i);
		else if(end < bgn)
			for(index_type i = bgn ; i > end ; i--)
				m_indeces.push_back(i);
	}

	size_type size() const noexcept  { return m_indeces.size(); }

	iterator begin() noexcept { return m_indeces.begin(); }
	const_iterator begin() const noexcept { return m_indeces.begin(); }
	const_iterator cbegin() noexcept { return m_indeces.cbegin(); }

	iterator end() noexcept { return m_indeces.end(); }
	const_iterator end() const noexcept { return m_indeces.end(); }
	const_iterator cend() const noexcept { return m_indeces.cend(); }

	reference operator[]( size_type pos ) { return m_indeces[pos]; }
	const_reference operator[]( size_type pos ) const { return m_indeces[pos]; }

private:

	std::vector<index_type> m_indeces;
};

}} // namespace mul::stdx

#endif // MUL_STDX_INDEX_RANGE_HPP_
