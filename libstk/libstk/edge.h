/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Edge class used by the surface classes to draw polygons.
 *     AUTHORS: Darren Hart
 *  START DATE: 2002/Dec/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EDGE_H
#define STK_EDGE_H

namespace stk
{
    // edge used for scanfill of polygons
    class edge
    {
    private:
        int ymax_, xbot_, xinc_num_, xinc_den_, xinc_, xinc_dir_;
    public:
        edge(int ymax, int xbot, int xinc_num, int xinc_den) :
                ymax_(ymax), xbot_(xbot), xinc_num_(xinc_num),
            xinc_den_(xinc_den), xinc_(0), xinc_dir_(((xinc_num>0)?1:-1))
        {}
            
        int ymax() const { return ymax_; }
        void ymax(int ymax) { ymax_ = ymax; }
        int xbot() const { return xbot_; }
        void xbot(int xbot) { xbot_ = xbot; }
        int xinc_num() const { return xinc_num_; }
        void xinc_num(int xinc_num) { xinc_num_ = xinc_num; }
        int xinc_den() const { return xinc_den_; }
        void xinc_den(int xinc_den) { xinc_den_ = xinc_den; }
        int xinc() const { return xinc_; }
        void xinc(int xinc) { xinc_ = xinc; }

        void step()
        {
            xinc_ += xinc_num_;
            while (abs(xinc_) >= xinc_den_)
            {
                xbot_ += xinc_dir_;
                if (xinc_dir_ > 0)
                    xinc_ -= xinc_den_;
                else
                    xinc_ += xinc_den_;
            }
        }

        bool operator<(const edge &ed) const
        {
            if (xbot_ < ed.xbot())
                return true;
            else
                return false;
        }
    };
}


#endif
