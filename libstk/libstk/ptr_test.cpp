#include <iostream>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

int main()
{
	int retval = 0;

	int* value = new int(1);

	shared_ptr<int> ptr_a(value);
	cout << "ptr_a.use_cout() = " << ptr_a.use_count() << endl;
	
	shared_ptr<int> ptr_b(value);
	cout << "ptr_a.use_cout() = " << ptr_a.use_count() << endl;
	cout << "ptr_b.use_cout() = " << ptr_b.use_count() << endl;
	
	return retval;
}
