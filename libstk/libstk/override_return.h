/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Return value override classes and operators, used to pass void() methods as slotes
 *              to signals.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Sep/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/
#ifndef OVERRIDE_RETURN_H
#define OVERRIDE_RETURN-H

template<class F, class V> 
class override_return_t
{
private:

    F f_;
    V v_;

public:

    typedef V result_type;

    override_return_t(F const & f, V const & v): f_(f), v_(v)
    {
    }

    result_type operator()()
    {
        f_();
        return v_;
    }

    result_type operator()() const
    {
        f_();
        return v_;
    }
};


template<class F, class V> override_return_t<F, V> 
override_return(F const &f, V const & v)
{
    return override_return_t<F, V>(f, v);
}


template<class R, class F, class L, class V>
override_return_t<boost::_bi::bind_t<R, F, L>, V>
operator,(boost::_bi::bind_t<R, F, L> const & f, V const & v)
{
    return override_return_t<boost::_bi::bind_t<R, F, L>, V>(f, v);
}


// end override return syntax

#endif
