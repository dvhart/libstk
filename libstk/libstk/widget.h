/***************************************************************************
                          stk_widget.h  -  description
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

#ifndef STK_WIDGET_H
#define STK_WIDGET_H

#include <string>
#include <map>
#include <list>
#include <sigc++/signal_system.h>
#include <boost/shared_ptr.hpp>
#include <SDL/SDL.h>

#include "stk_theme.h"

namespace stk {
	class widget;
	typedef boost::shared_ptr<widget> widget_ptr;
	typedef std::map<std::String, widget_ptr> widget_map;
	
	class widget : public SigC::Object {
	public:
		widget(SDL_Rect &rect);
		virtual ~widget();
		void init(SDL_Rect &rect);

		inline void set_app(app &val) { _app = val; };
		inline app &get_app(void) { return _app; };

		virtual void set_focus(bool val);
		inline bool get_focus() { return _focus; };

		inline void set_focusable(bool val) { _focusable = val; };
		inline bool get_focusable() { return _focusable; };

		inline void set_parent(widget &val) { _parent = &val; };
		inline widget &get_parent() { return *_parent;} ;

		// FIXME: can we set the rect, what happens to the surfaces ?
		inline void set_rect(SDL_Rect &val) { _rect = &val; };
		inline SDL_Rect get_rect() { return *_rect; };

		inline void set_redraw(bool val) { _redraw = val; };
		inline bool get_redraw() { return _redraw; };

		inline void set_visible(bool val) { _visible = val; };
		inline bool get_visible() { return _visible; };

		inline void set_theme(theme &val) { _theme = &val; };
		inline theme &get_theme() { return *_theme; };

		virtual void set_text(const std::string &val);
		virtual std::string get_text();

		inline int get_type(void) { return _type; };

		inline void set_name(const std::string &val) { _name = val; };
		inline std::string get_name(void) { return _name; };

		// util methods
		virtual int blit(surface &screen);	  // blit this surface onto the passed surface "screen"
		virtual int add_child(widget &val);
		int activate();
		void register_key(SDLKey k, keyslot acall);
		virtual int draw();                             // update this surface
		virtual void handle_event(SDL_Event &event);

		// the new signal system
		SigC::Signal1<void, widget&>         on_focus;     // void sig(widget&)
		SigC::Signal1<void, widget&>         on_unfocus;   // void sig(widget&)
		SigC::Signal1<void, widget&>         on_activate;  // void sig(widget&)
		SigC::Signal2<bool, widget&, SDLKey> on_keypress;  // bool sig(widget&, SDLKey)

	protected:
		int _type;                
		std::string _name;

		SDL_Rect *_rect;
		SDL_Rect *_clip_rect;
		surface _bg_surface;
		surface _fg_surface;

		int _active; // used as a timer
		bool _focus;
		bool _focusable;
		bool _redraw;
		bool _visible;

		app *_app;
		widget *parent;
		theme *_theme;

		map<SDLKey, keyslot> keybindings;
	};

} // namespace stk

#endif
