#include "widget.h"
#include "container.h"
#include "app.h"

#include <iostream>
#include <typeinfo>


typedef boost::shared_ptr<stk::app>    App;
typedef boost::shared_ptr<stk::state>  State;
typedef boost::shared_ptr<stk::widget> Widget;

int main(int argc,char* arv[])
{
	App test_app(new stk::app);  // (new stk::app) as an argument is weird
	State test_state(new stk::state(testapp));
	testapp->current_state(teststate);
	
	Widget a(new stk::widget(teststate)) ,b(new stk::widget(teststate));
	teststate->add(a);
	teststate->add(b);
}
