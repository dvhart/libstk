#include "container.h"
#include "exceptions.h"

container::container() :
        _iter(childs.begin())
{}

container::~container()
{}

void container::add
    (std::string name, stk::widget &widget)
{
    typedef child_map::iterator i;

    std::pair<std::string, stk::widget *> p(name, &widget);
    i iter = _childs.insert(p);

    if (!iter.second)
        throw stk::exception; /* could not insert */
}

void container::erase(std::string name)
{
    if (_childs.erase(name) != 1)
        throw stk::exception; /* no or more then one widget erased */
}

void container::erase(stk::widget &widget)
{
    typedef child_map::const_iterator ci;

    for (ci iter = _childs.begin(); iter != _childs.end(); iter++)
        if (iter->second == &widget)
            if (_childs.erase(iter) != 1)
                throw stk::exception; /* no or more then one widget erased */
}

stk::widget &lookup_widget(std::string name)
{
    typedef child_map::const_iterator ci;

    ci iter = _childs.find(name);
    if (iter = _childs.end()) /* no pair found */
        throw stk::error_message_exception("stk::container::lookup_widget(\""+name+"\"): no such widget found!");

    return *(iter->second);
}

stk::widget &container::prev()
{
    if (_iter == childs.begin())
        _iter = childs.end();
    else
        _iter--;

    return *(_iter->second);
}

stk::widget &container::next()
{
    if (_iter == childs.end())
        _iter = childs.begin();
    else
        _iter++;

    return *(_iter->second);
}
