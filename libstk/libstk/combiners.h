#include <vector>

namespace stk::combiner
{
	struct and 
	{ 
		bool operator ()(InputIterator first, InputIterator last) const
		{
			if (first == last) return true; // no connections, so we didn't fail
			while (first != last)
			{
				if (! *first) return false; // one failed, so return false
				++first;
			}
			return true; // no connections failed
		}
	}
	
	struct or
	{ 
		bool operator ()(InputIterator first, InputIterator last) const
		{
			if (first == last) return true; // no connections, so we didn't fail
			while (first != last)
			{
				if (*first) return true; // one succeeded, so return true
				++first;
			}
			return false; // no connections succeeded
		}
	}

	// FIXME: make this a template so we can use anything with operator+= defined
	struct add
	{ 
		int operator ()(InputIterator first, InputIterator last) const
		{
			int sum;
			if (first == last) return 0;
			while (first != last)
			{
				sum += *first;
				++first;
			}
			return sum;
		}
	}

}
