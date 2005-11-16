/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Signal combiner objects.  They combine the results of all the
 *              slots of a signal in a specific way and return the result.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Mar/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_COMBINERS_H
#define STK_COMBINERS_H

#include <vector>

namespace stk
{
    namespace combiner
    {

        template <typename T>
        struct logical_and
        {
            typedef T result_type;

            template <typename InputIterator>
            T operator ()(InputIterator first, InputIterator last) const
            {
                if (first == last)
                    return false; // no connections, so we didn't handle it
                bool ret = true;
                while (first != last)
                {
                    if (! *first) ret = false; // one failed, so return false
                    ++first;
                }
                return ret; // no connections failed
            }
        };

        template <typename T>
        struct logical_or
        {
            typedef T result_type;

            template <typename InputIterator>
            T operator ()(InputIterator first, InputIterator last) const
            {
                if (first == last)
                    return false; // no connections, so we didn't handle it
                bool ret = false;
                while (first != last)
                {
                    if (*first) ret = true; // one succeeded, so return true
                    ++first;
                }
                return ret; // no connections succeeded
            }
        };

        template <typename T>
        struct add
        {
            typedef T result_type;

            template <typename InputIterator>
            T operator ()(InputIterator first, InputIterator last) const
            {
                T sum;
                if (first == last)
                    return T(0);
                while (first != last)
                {
                    sum += *first;
                    ++first;
                }
                return sum;
            }
        };

        template <typename T>
        struct subtract
        {
            typedef T result_type;

            template <typename InputIterator>
            T operator ()(InputIterator first, InputIterator last) const
            {
                T sum;
                if (first == last)
                    return T(0);
                while (first != last)
                {
                    sum -= *first;
                    ++first;
                }
                return sum;
            }
        };


        template <typename T>
        struct maximum
        {
            typedef T result_type;

            template <typename InputIterator>
            T operator ()(InputIterator first, InputIterator last) const
            {
                T max = *first;
                if (first == last)
                    return T(0);
                while (first != last)
                {
                    if (*first > max)
                        max = *first;
                    ++first;
                }
                return max;
            }
        };

        template <typename T>
        struct minimum
        {
            typedef T result_type;

            template <typename InputIterator>
            T operator ()(InputIterator first, InputIterator last) const
            {
                T min = *first;
                if (first == last)
                    return T(0);
                while (first != last)
                {
                    if (*first < min)
                        min = *first;
                    ++first;
                }
                return min;
            }
        };

    } // namespace combiner
} // namepsace stk

#endif
