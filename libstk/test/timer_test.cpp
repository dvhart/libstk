#include <iostream>

#include <libstk/time_value.h>
#include <libstk/posix_time.h>
#include <libstk/logging.h>

using namespace stk;

int main()
{
    logger::get()->add_target(&std::cout, LL_Info);
    std::cout << "Libstk timer test, please wait 2 seconds" << std::endl;

    time_value a = get_current_time_value();
    usleep(1000000 * 2); // 2 seconds
    time_value b = get_current_time_value();
    time_value c = b - a;

    std::cout << "Time elapsed " << c.seconds() << " s " << c.millis() << " ms" << std::endl;
    std::cout << "If this is much different than 2 s 0 ms, then the timer implementatino is broken" << std::endl;

    INFO("Testing rapid differences");
    time_value prev = get_current_time_value();
    time_value cur;
    time_value diff;
    for (int i = 0; i < 100; i++)
    {
        usleep(1);
        cur = get_current_time_value();
        diff = cur - prev;
        prev = cur;
        INFO("DIFF SECS=" << diff.seconds() << "     MILLIS=" << diff.millis());
        if (diff.seconds() < 0 || diff.millis() < 0)
            ERROR("\n**********\nNEGATIVE TIME SHIFT\n**********");
    }
    return 0;
}
