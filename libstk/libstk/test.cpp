#include "widget.h"
#include "container.h"
#include "app.h"

#include <iostream>
#include <typeinfo>

int main(int argc,char* arv[])
{
	boost::shared_ptr<stk::app> testapp(new stk::app);
	boost::shared_ptr<stk::state> teststate(new stk::state(testapp));
	testapp->current_state(teststate);
	
	boost::shared_ptr<stk::widget> a(new stk::widget(teststate)) ,b(new stk::widget(teststate));
	teststate->add(a);
	teststate->add(b);
	
}
