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

#include <libstk/read_dir.h>
#include <libstk/exceptions.h>

namespace stk
{


    std::vector<dir_entry::ptr> read_dir(const std::string& dirname, const std::string& select)
    {
        DIR * fontdir;
        std::vector<dir_entry::ptr> dir_contents;

        fontdir = opendir(dirname.c_str());
        if (!fontdir)
        {
            throw error_message_exception(std::string("could not open directory ") + dirname);
        }

        struct dirent * cur_entry;
        while ((cur_entry = readdir(fontdir)) != NULL)
        {
            dir_contents.push_back(dir_entry::create(cur_entry));
        }
        closedir(fontdir);

        // select the ones we want
        std::vector<dir_entry::ptr> selected_files;
        std::vector<dir_entry::ptr>::iterator dir_iter = dir_contents.begin();
        for (; dir_iter != dir_contents.end(); dir_iter++)
        {
            if (strncasecmp((*dir_iter)->filename().c_str(), select.c_str(), select.length()) == 0)
                selected_files.push_back(*dir_iter);
        }

        dir_entry_lt lt;
        sort(selected_files.begin(), selected_files.end(), lt);

        return selected_files;
    }

} // namespace stk

