/***************************************************************************
                          stk_app.cpp  -  description
                             -------------------
    begin                : Sat Apr 27 2002
    copyright            : (C) 2002 by Darren Hart
    email                : dvhart@byu.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "stk.h"

using namespace stk;
using std::cout;
using std::endl;
using std::string;

app::app() {
  screen = NULL;
}

app::~app() { }

int app::init(Uint32 sdl_flags) {
  // set some defaults
  if (!sdl_flags)
    sdl_flags = SDL_INIT_VIDEO;

  if (SDL_Init(sdl_flags | SDL_INIT_TIMER) < 0) {
    // we always pass SDL_INIT_TIMER since exec needs it
    std::cout << "app::init - Unable to init SDL: " << SDL_GetError() << std::endl;
    return 1;
  }
  // SDL cleanup on exit (ESC or window close)
  atexit(SDL_Quit);
  return 0;
}

int app::init_video(int width, int height, int bpp, Uint32 sdl_flags) {
  // set some defaults
  if (!width)
    width = 640;
  if (!height)
    height = 480;
  if (!bpp)
    bpp = 32;
  if (!sdl_flags)
    sdl_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;

  screen = SDL_SetVideoMode(width, height, bpp, sdl_flags);

  SDL_ShowCursor(SDL_DISABLE);

  return 0;
}

int app::run() {
  int done = 0;
  SDL_Event event;
  SDL_Event new_event;
  widget *cur_widget;
  int found;
  int keydown = 0, has_event;
  bool handled = false;

  while (!done) {
    // handle any events
    if ((has_event = SDL_PollEvent(&new_event)) || keydown) {
      if (has_event && good_event(&new_event)) {
	memcpy(&event, &new_event, sizeof(SDL_Event));
	keydown = 0;
      } else if (!keydown) continue;

      found = 0;

      if (event.type == SDL_QUIT) {
	done = 1;
      }
      
      if (event.type == SDL_KEYUP) {
	// cancel the last keydown
	if (event.key.keysym.sym == keydown) {
	  keydown = 0;
	}
	continue;
      } else if (event.type == SDL_KEYDOWN) {
	if (keydown) {
	  SDL_Delay(50);
	}
	// we are only responding to keystrokes at this point in time
	keydown = event.key.keysym.sym;
	// handle global app keys
	// give the active widget a chance to respond to event first
	// the widget will pass the event on to its parent and so on
	// until someone responds.  If no widget responds, then we do a
	// default action definded below.
	
	// pass the event to the current state
	if (!focused_widget->handle_event(&event)) {
	  // check for the registeres global keys created with
	  // FIXME: check if the return value is working
	  handled = keybindings[event.key.keysym.sym].call(0);
	  if (handled) {
	    cout << "************ global keybinding succeded ***********" << endl;
	    continue;	  
	  }

	  // default handlers
	  switch (event.key.keysym.sym) {
	  case SDLK_ESCAPE:
	    done = 1;
	    break;
	    
	  case SDLK_RETURN:
	    focused_widget->activate();
	    break;
	    
	  // get the previous widget
	  case SDLK_LEFT:
	  case SDLK_UP:
	    // find a focusable widget
	    cur_widget = focused_widget;
	    while (!found) {
	      if (cur_widget->get_prev()) {
		cur_widget = cur_widget->get_prev();
		if (cur_widget == focused_widget) {
		  std::cerr << "no prev focusable widget could be found" << std::endl;
		  break;
		}
	      } else {
		//cout << "no prev siblings" << endl;
		cur_widget = cur_state->get_child();
		// go to the last widget in the state - loop back around
		while (cur_widget->get_next()) {
		  cur_widget = cur_widget->get_next();
		}
	      }
	      
	      if (cur_widget->get_focusable()) { 
		found = 1; 
	      } else {
		//cout << "widget not focusable (" << cur_widget->get_name() << ")" << endl;
	      }
	    }
	    if (cur_widget != focused_widget) {
	      //cout << "found a new focusable widget -- " << cur_widget->get_name() << endl;
	    }
	    focused_widget->set_focus(false);
	    cur_widget->set_focus(true);
	    break;
	    
	    // get the next widget
	  case SDLK_RIGHT:
	  case SDLK_DOWN:
	    // find a focusable widget
	    cur_widget = focused_widget;
	    while (!found) {
	      if (cur_widget->get_next()) {
		cur_widget = cur_widget->get_next();
	      } else {
		//cout << "no next siblings" << endl;
		cur_widget = cur_state->get_child();
	      }
	      
	      if (cur_widget->get_focusable()) { 
		found = 1; 
	      } else {
		//cout << "widget not focusable" << endl;
	      }
	    }
	    
	    focused_widget->set_focus(false);
	    cur_widget->set_focus(true);
	    break;
	    
	  default:
	    std::cerr << "app::run() - unknown event - ignoring" << std::endl;

	    // perhaps call the keypress signal here
	    // FIXME: resolve what to do about the register_key model
	    // vs. the libsigc model
	    focused_widget->on_keypress(*focused_widget, event.key.keysym.sym);
	    break;
	  }
	}
      }
    }

    // draw the current state
    if (cur_state->get_redraw()) cur_state->draw();
    cur_state->blit(screen);
    // draw the focused widget (making sure it is on top)
    // FIXME: should we check that it is visible? != cur_state, can this cause infinite recursion?
    if (focused_widget->get_type() == STK_CLASS_LIST_ITEM) {
      listbox *tlist = (listbox *)((list_item *)focused_widget)->get_listbox();
      if (tlist->get_redraw()) 
	tlist->draw();
      tlist->blit(screen);
    }
    if (focused_widget->get_redraw()) 
      focused_widget->draw();
    focused_widget->blit(screen);

    SDL_Flip(screen);

    // hop off the processor for a few ms
    if (!SDL_PollEvent(NULL)) SDL_Delay(STK_TICK);
  }

  return 0;
}

int app::set_state(const state &val) {
  state_iter = find(states.begin(), states.end(), val.get_name());
  if (state_iter == states.end()) return 1;
  set_focused(state_iter.data());
  return 0;
}

int app::set_state(const string &name) {
  state_iter = find(states.begin(), states.end(), name);
  if (state_iter == states.end()) return 1;
  set_focused(state_iter.data());
  return 0;
}

int app::add_state(const state &val) {
  states[val.name] = &val;
  if (states.count() == 1) {
    state_iter = states.begin();
  }
  return 0;
}

int app::load_theme(const string &name) {
}

widget_ptr app::lookup_widget(const string &name) {
  cout << "lookup_widget start" << endl;

  for (state::state_map::iterator iter = states.begin(); 
       iter != states.end(); iter++) {
    if (iter->get_name() == name) return iter.data();
    widget_ptr the_widget = iter->lookup_widget(name);
    if (the_widget) return the_widget;
  }
  
  cout << "app::lookup_widget - ERROR failed to find widget '" << 
    name << "'" << endl;
  return NULL;
}

bool app::good_event(SDL_Event *event) {
  if ((event->type == SDL_QUIT) ||
      (event->type == SDL_KEYDOWN) ||
      (event->type == SDL_KEYUP)) {
    return true;
  }
  return false;
}

void app::set_focused(widget &val) {
  if (&val == focused_widget) return;
  focused_widget->set_focused(false);
  val->set_focused(true);
  focused_widget = &val;
}

