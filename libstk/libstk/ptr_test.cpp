/******************************************************************************
 *    FILENAME:  
 * DESCRIPTION: 
 *     AUTHORS: First Last, First Last, etc.
 *  START DATE: DD/MMM/YYYY  LAST UPDATE: DD/MMM/YYY
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

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
