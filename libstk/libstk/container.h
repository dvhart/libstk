#ifndef _STK_CONTAINER_H_
#define _STK_CONTAINER_H_

#include <map>
#include <vector>

#include "widget.h"

namespace stk
{

class container : public stk::widget
{
private:
    typedef std::vector<Twidget_ptr> Tchilds;
    
	Tchilds childs_;	

public:
    container(app* app,widget* parent);
    ~container();

    void add(Twidget_ptr widget);
    void erase(Twidget_ptr widget);

    Twidget_ptr prev();
    Twidget_ptr next();

    Twidget_ptr lookup_widget(std::string name);
};

} // namespace stk

#endif
