/***************************************************************************
			  app.cpp  -  stk::app implementation
			     -------------------
    begin		: Sat Apr 27 2002
    copyright	    : (C) 2002 by Darren Hart
    email		: dvhart@byu.edu
 ***************************************************************************/

/***************************************************************************
 *									 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									 *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>

#include "stk.h"

namespace stk
{

app::app() : _screen(NULL)
{
}

app::~app()
{
}

void app::init(Uint32 sdl_flags)
{
	/* set some defaults */
	if (!sdl_flags) sdl_flags = SDL_INIT_VIDEO;

	if (SDL_Init(sdl_flags | SDL_INIT_TIMER) < 0) /* we always pass SDL_INIT_TIMER since exec needs it */
		throw stk::error_exception("app::init - Unable to init SDL: "+SDL_GetError());

	/* SDL cleanup on exit (ESC or window close) */
	atexit(SDL_Quit);
}

void app::init_video(int width, int height, int bpp, Uint32 sdl_flags)
{
	/* set some defaults */
	if (!width)     width = 640;
	if (!height)    height = 480;
	if (!bpp)       bpp = 32;
	if (!sdl_flags) sdl_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;

	_screen = SDL_SetVideoMode(width, height, bpp, sdl_flags);
	if (!_screen == NULL)
		throw stk::error_exception("app::init_video - Unable to set videomode: "+SDL_GetError());

	SDL_ShowCursor(SDL_DISABLE);
}

int app::run()
{
	bool done = false;


	SDL_Event event;
	SDL_Event new_event;

	stk::widget *cur_widget;

	int found, has_event, keydown = 0;
	bool handled = false;

	while (!done)
	{
		/* at first event-handling */
		if ((has_event = SDL_PollEvent(&new_event)) || keydown)
		{
			if (has_event && good_event(&new_event))
			{
				memcpy(&event, &new_event, sizeof(SDL_Event));
				keydown = 0;
			} else
				if (!keydown) continue;

			found = 0;

			switch (event.type)
			{
			case SDL_QUIT:
				done = true;
				break;

			case SDL_KEYUP:
				/* cancel the last keydown */
				if (event.key.keysym.sym == keydown) keydown = 0;
				continue;
				break;

			case SDL_KEYDOWN:
				/* key pressed */
				if (keydown) SDL_Delay(50);

				/* we are only responding to keystrokes at this point in time */
				keydown = event.key.keysym.sym;

				/* handle global app keys
				 * give the active widget a chance to respond to event first
				 * the widget will pass the event on to its parent and so on
				 * until someone responds.  If no widget responds, then we do a
				 * default action definded below.
				 */

				/* pass the event to the current state */
				if (!focused_widget->handle_event(&event))
				/* check for the registeres global keys created with */
				/* FIXME: check if the return value is working */
				{
					handled = keybindings[event.key.keysym.sym].call(0);
					if (handled)
					{
						std::cout << "************ global keybinding succeded ***********" << std::endl;
						continue;
					}

					/* default handlers */
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						done = true;
						break;

					case SDLK_RETURN:
						focused_widget->activate();
						break;

					/* get the previous widget */
					case SDLK_LEFT:
					case SDLK_UP:
						/* FIXME: to be written */
						break;

					/* get the next widget */
					case SDLK_RIGHT:
					case SDLK_DOWN:
						/* FIXME: to be written */
						break;

					default:
						std::cerr << "app::run() - unknown event - ignoring" << std::endl;
						/* perhaps call the keypress signal here
						 * FIXME: resolve what to do about the register_key model
						 * vs. the libsigc model
						 */
						focused_widget->on_keypress(*focused_widget, event.key.keysym.sym);
						break;
					}

				}
			}
		} // event handling

		/* draw the current state */
		if (_state->get_redraw()) _state->draw();
		_state->blit(screen);

		/* draw the focused widget (making sure it is on top) */
		// FIXME: should we check that it is visible? != _state, can this cause infinite recursion?
		if (_focused_widget->get_type() == STK_CLASS_LIST_ITEM)
		{
			stk::listbox *tlist = (stk::listbox *)((stk::list_item *)_focused_widget)->get_listbox();
			if (tlist->get_redraw()) tlist->draw();
			tlist->blit(screen);
		}
		if (focused_widget->get_redraw()) focused_widget->draw();
		focused_widget->blit(screen);

		SDL_Flip(screen);

		/* hop off the processor for a few ms */
		if (!SDL_PollEvent(NULL)) SDL_Delay(STK_TICK);

	} // while (!done)
}

void app::set_state(const stk::state &state)
{
	state_iter = find(states.begin(), states.end(), state.get_name());
	if (state_iter == states.end()) return 1;
	set_focused(state_iter.data());
}

void app::set_state(const std::string &name)
{
	state_iter = find(states.begin(), states.end(), name);
	if (state_iter == states.end()) return 1;
	set_focused(state_iter.data());
}

void app::add_state(const stk::state &state)
{
	states[val.name] = &state;
	if (states.count() == 1) state_iter = states.begin();
}

void app::set_theme(const std::string &name)
{
}

stk::widget *app::get_widget(const std::string &name)
{
	std::cout << "lookup_widget start" << std::endl;

	for (state::state_map::iterator iter = states.begin(); iter != states.end(); iter++)
	{
		if (iter->get_name() == name) return iter.data();
		stk::widget *the_widget = iter->get_widget(name);

		if (the_widget) return the_widget;
	}

	throw stk::error_exception("app::lookup_widget - ERROR failed to find widget '"+name+"'");
}

bool app::good_event(SDL_Event *event)
{
	if ((event->type == SDL_QUIT) ||
	    (event->type == SDL_KEYDOWN) ||
	    (event->type == SDL_KEYUP))
		return true;

	return false;
}

void app::set_focused(stk::widget &widget) {
	if (&widget == _focused_widget) return;

	focused_widget->set_focused(false);
	widget.set_focused(true);
	focused_widget = &widget;
}

