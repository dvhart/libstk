#ifndef STK_COMBINERS_H
#define STK_COMBINERS_H

#include <vector>

namespace stk
{
	namespace combiner
	{
		
		template <typename T>
		struct logical_and 
		{ 
			typedef T result_type;

			template <typename InputIterator>
			T operator ()(InputIterator first, InputIterator last) const
			{
				if (first == last) return true; // no connections, so we didn't fail
				while (first != last)
				{
					if (! *first) return false; // one failed, so return false
					++first;
				}
				return true; // no connections failed
			}
		};

		template <typename T>
		struct logical_or
		{ 
			typedef T result_type;

			template <typename InputIterator>
			T operator ()(InputIterator first, InputIterator last) const
			{
				if (first == last) return true; // no connections, so we didn't fail
				while (first != last)
				{
					if (*first) return true; // one succeeded, so return true
					++first;
				}
				return false; // no connections succeeded
			}
		};

		template <typename T>
		struct add
		{ 
			typedef T result_type;

			template <typename InputIterator>
			T operator ()(InputIterator first, InputIterator last) const
			{
				T sum;
				if (first == last) return T(0);
				while (first != last)
				{
					sum += *first;
					++first;
				}
				return sum;
			}
		};
		
		template <typename T>
		struct subtract
		{ 
			typedef T result_type;

			template <typename InputIterator>
			T operator ()(InputIterator first, InputIterator last) const
			{
				T sum;
				if (first == last) return T(0);
				while (first != last)
				{
					sum -= *first;
					++first;
				}
				return sum;
			}
		};
		
	
		template <typename T>
		struct maximum
		{ 
			typedef T result_type;

			template <typename InputIterator>
			T operator ()(InputIterator first, InputIterator last) const
			{
				T max = *first;
				if (first == last) return T(0);
				while (first != last)
				{
					if (*first > max) max = *first;
					++first;
				}
				return max;
			}
		};
	
		template <typename T>
		struct minimum
		{ 
			typedef T result_type;

			template <typename InputIterator>
			T operator ()(InputIterator first, InputIterator last) const
			{
				T min = *first;
				if (first == last) return T(0);
				while (first != last)
				{
					if (*first < min) min = *first;
					++first;
				}
				return min;
			}
		};

	}
}

#endif
