/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: utility to read a directory and find files
 *     AUTHORS: Vernon Mauery
 *  START DATE: 2003/Oct/08
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_READDIR_H
#define STK_READDIR_H

#include <sys/types.h>
#include <dirent.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

namespace stk
{
    class dir_entry
    {
        public:
            typedef boost::shared_ptr<dir_entry> ptr;

            static dir_entry::ptr create(dirent * d)
            {
                dir_entry::ptr ret(new dir_entry(d));
                return ret;
            }

            ~dir_entry()
            {
            }

            long inode() const { return dir_entry_.d_ino; }
            unsigned char type() const { return dir_entry_.d_type; }
            std::string filename() const { return std::string(dir_entry_.d_name); }
            bool operator < (const dir_entry& rhs) const
            {
                return filename() < rhs.filename();
            }

        protected:
            dir_entry(dirent *d)
            {
                dir_entry_ = *d;
            }

            dirent dir_entry_;
    };

    std::vector<dir_entry::ptr> read_dir(const std::string& dirname, const std::string& select="");

}

#endif // STK_READDIR_H
