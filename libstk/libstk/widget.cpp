/***************************************************************************
                          stk_widget.cpp  -  description
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

#include <string.h>
#include <malloc.h>

#include "widget.h"
#include "app.h"
#include "stk_util.h"

using namespace stk;
using std::cout;
using std::string;
using std::endl;

widget::widget(SDL_Rect *new_rect = NULL) : public SigC::Object {
  // NOTE: if new_rext is NULL, widgets need their parent to determine their 
  // rect size.  Said widgets *MUST* call widget::init after they have 
  // determined their rect!
  if (new_rect) init(new_rect);
  else {
    bg_surface = NULL;
    fg_surface = NULL;
  }

  clip_rect = NULL;
  active = 0;
  focus = false;
  focusable = false;
  redraw = true;
  visible = true;
  type = STK_CLASS_WIDGET;
}

widget::~widget() {
  if (bg_surface) SDL_FreeSurface(bg_surface);
  if (fg_surface) SDL_FreeSurface(fg_surface);
  if (clip_rect) delete clip_rect;
}

void widget::init(SDL_Rect * trect) {
  SDL_Surface *temp_surface;

  rect = new_rect(trect->x, trect->y, trect->w, trect->h);

  temp_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, rect->w, rect->h,
				      32, RMASK, GMASK, BMASK, AMASK);
  bg_surface = SDL_DisplayFormatAlpha(temp_surface);
  SDL_FreeSurface(temp_surface);
  SDL_SetAlpha(bg_surface, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);

  temp_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, rect->w, rect->h,
				      32, RMASK, GMASK, BMASK, AMASK);
  fg_surface = SDL_DisplayFormatAlpha(temp_surface);
  SDL_FreeSurface(temp_surface);
  SDL_SetAlpha(fg_surface, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
}

void widget::set_focus(bool val) {
  if (focusable) {
    if (focus != val) redraw = true;
    if ((focus = val)) _app->set_focused(this); // yes, I meant "="
    active = 0;
    if (val) on_focus(*this);
    else on_unfocus(*this);
  }
}

// implement a transparent interface
// see Design Principles - Composite
// FIXME: should we always return int on set operations ?
void widget::set_text(string const& val) { }
string widget::get_text() { return NULL; }
int widget::add_child(widget_ptr val) { return 1; }
// del_child(const string &name);  ???

int widget::activate() {
  active = STK_ACTIVE_TIME;
  redraw = true;
  on_activate(*this);
  return 0;
}

int widget::blit(SDL_Surface * screen) {
  // blit the local surface to the screen
  if (bg_surface && SDL_BlitSurface(bg_surface, NULL, screen, rect) < 0) {
    cerr << "widget: Failed to blit bg_surface to screen" << endl;
  }

  // blit the text surface to the screen
  if (fg_surface && SDL_BlitSurface(fg_surface, NULL, screen, rect) < 0) {
    cerr << "widget: Failed to blit fg_surface to screen" << endl;
  }

  // blit all the child widgets
  // skip the focused on, the app will draw it last
  widget *cur_widget;
  cur_widget = this->get_child();
  while (cur_widget) {
    if (cur_widget->get_visible() && !cur_widget->get_focus()) {
      if (cur_widget->get_redraw()) 
	cur_widget->draw();
      cur_widget->blit(screen);
    }
    cur_widget = cur_widget->next;
  }

  if (active) {
    if (!--active) {
      redraw = true;
    }
  }
  return 0;
}

int widget::draw() {
  // draw the border and background, called by derived classes
  int bg_radius = MIN(my_theme_item->get_bg_radius(),rect->h/2);
  int border_radius = MIN(my_theme_item->get_border_radius(),rect->h/2);
  int border_width = my_theme_item->get_border_width();

  // prepare for pixel level editting of the surfaces
  slock(bg_surface);
  slock(fg_surface);

  // clear the surface
  SDL_FillRect(bg_surface, NULL, (SDL_ALPHA_TRANSPARENT << ASHIFT));
  SDL_FillRect(fg_surface, NULL, (SDL_ALPHA_TRANSPARENT << ASHIFT));

  Uint32 bg_color, fg_color;

  // determine the current font and colors
  if (active) {
    bg_color = my_theme_item->get_bg_active_color();
    fg_color = my_theme_item->get_border_active_color();
    cur_font = my_theme_item->get_font_active();
  } else if (focus) { 
    bg_color = my_theme_item->get_bg_focus_color();
    fg_color = my_theme_item->get_border_focus_color();
    cur_font = my_theme_item->get_font_focus();
  } else {
    bg_color = my_theme_item->get_bg_color();
    fg_color = my_theme_item->get_border_color();
    cur_font = my_theme_item->get_font();
  }

  // fill the backgound
  SDL_Rect *t_rect = new_rect(0, 0, rect->w, rect->h);
  fill_rect(bg_surface, t_rect, bg_radius, bg_color);

  // outline the box
  if (border_width > 0) {
    draw_rect(fg_surface, t_rect, border_width, border_radius, fg_color);
  }
  delete t_rect;

  sulock(fg_surface);
  sulock(bg_surface);

  redraw = false;

  return 0;
}

void widget::handle_event(SDL_Event * event) {
  if (keybindings[event->key.keysym.sym].call(0)) return;
  if (on_keypress(*this, event->key.keysym.sym)) return;
  get_parent()->handle_event(event);
}

// global keybinding registration
void widget::register_key(SDLKey k, keyslot acall) {
  keybindings[k] = acall;
}






// debug methods
void widget::print_siblings(widget *start) {
  cout << "print_siblings()"<<endl;
  widget *w = start;
  while (w) {
    cout<<"name="<<w->get_name()<<", text="<<w->get_text()<<hex
	<<", prev="<<w->get_prev()<<"addr="<<w<<", next="<<w->get_next()<<dec<<endl;
    w = w->get_next();
  }
  cout << "end print_siblings()"<<endl;
}

