#ifndef STK_CONTAINER_H
#define STK_CONTAINER_H

#include <map>
#include <vector>
#include <boost/smart_ptr.hpp>
#include "widget.h"

namespace stk
{
class container;
typedef boost::shared_ptr<container> Tcontainer_ptr;
class container : public stk::widget
{
private:
    typedef std::vector<Twidget_ptr> Tchilds;
	Tchilds childs_;	
public:
	virtual bool is_container() { return true;} 
	virtual boost::weak_ptr<widget> get_active_child()  // called when the tree parser for widget cycling switches focus to a container
			{ return  *childs_.begin(); }// default behaviour for a container is to switch into the FIRST child first 
    container(boost::weak_ptr<state> state,boost::weak_ptr<container> parent);
    ~container();

    void add(Twidget_ptr item);
    void remove(Twidget_ptr item);
};

} // namespace stk

#endif
