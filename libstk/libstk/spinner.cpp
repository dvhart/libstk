#include "libstk/list.h"
#include "libstk/spinner.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{
	spinner::ptr spinner::create(container::ptr parent, const rectangle& rect)
	{
		spinner::ptr new_spinner(new spinner(parent, rect));
		parent->add(new_spinner);
		return new_spinner;
	}

	spinner::spinner(container::ptr parent, const rectangle& rect) : list(parent, rect)
	{
		cout << "spinner::spinner()" << endl;
		focusable(true);
	}

	spinner::~spinner()
	{
	}

}
