#include "widget.h"
#include "container.h"
#include "app.h"

#include <iostream>
#include <typeinfo>

// Demoed here, not final location!
template <typename T>
struct noop_deleter
{
	void operator()(T* type) {}
};

template <typename T>
boost::shared_ptr<T> ptr_to_stack(T *ptr) { return boost::shared_ptr<T>(ptr,noop_deleter<T>());} 

/* DARRRREN!:) 1. against naming convention 2. meaningless names, the typedefs should be
app_ptr, state_ptr, widget_ptr  some of these are already defined!
typedef boost::shared_ptr<stk::app>    App;
typedef boost::shared_ptr<stk::state>  State;
typedef boost::shared_ptr<stk::widget> Widget;
*/
	
void sig_test()
{
	std::cout << "yeahha\n";
}

int main(int argc,char* arv[])
{
	stk::application test_app;
	stk::state test_state(ptr_to_stack(&test));
	/*App test_app(new stk::app);  // (new stk::app) as an argument is weird
	State test_state(new stk::state(test_app));
	testapp->current_state(teststate); */
	test.current_state(ptr_to_stack(&test_state));
	
	stk::widget a(ptr_to_stack(&test_state)),b(ptr_to_stack(&test_state));
	test_state.add(ptr_to_stack(&a));
	test_state.add(ptr_to_stack(&b));
	
	a.on_focus.connect(&sig_test);
	a.on_focus();
}
