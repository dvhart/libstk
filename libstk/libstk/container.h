#ifndef _STK_CONTAINER_H_
#define _STK_CONTAINER_H_

#include <map>
#include <string>

#include "stk.h"

namespace stk
{

class container : public stk::widget
{
private:
    typedef std::map<std::string, stk::widget *> child_map;

    child_map _childs;
    child_map::const_iterator _iter;

public:
    container();
    ~container();

    void add
        (std::string name, stk::widget &widget);
    void erase(std::string name);
    void erase(stk::widget &widget);

    stk::widget &prev();
    stk::widget &next();

    stk::widget &lookup_widget(std::string name);
};

} // namespace stk
