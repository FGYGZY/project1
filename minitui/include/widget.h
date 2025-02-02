#ifndef WIDGET_H__
#define WIDGET_H__

#include "geometry.h"
#include "event.h"
#include "ui.h"
#include <debug.h>

struct tui_frame;

struct tui_widget {
  char name[64];

  tui_rect area;
  bool updated;
  bool instaniated;
  bool reset_block;
  bool hovers;
  bool focused;

  tui_widget *parent;
  tui_widget *unproxy;
  tui_widget *frame;
  int retcode;
  int livecnt;
  bool deleted;
  std::vector<tui_widget *> children;
  
  static void delete_widget(tui_widget *widget, bool hard_delete);
  

  tui_widget(tui_rect area=global_rect, const char *name=NULL, tui_widget *parent=NULL);
  // forbids copying and assignment
  tui_widget(const tui_widget &) = delete;
  tui_widget& operator = (const tui_widget &) = delete;

  tui_point position_interpreter(tui_point point) const;
  tui_point position_mapper(tui_point local_point) const;
  tui_widget *create_widget(tui_widget *widget);
  tui_widget *add_widget(tui_widget *widget);
  tui_widget *set_attrs(std::vector<tui_fmt> attrs);
  void remove_widget(tui_widget *widget);
  void transform_widget(tui_widget *target);
  void proxize_widget(tui_widget *proxy);
  void reset_area(tui_rect area);

  // change the point from global to local
  tui_point gbl_point_interpreter(tui_point point) const;
  // change the point from local to global
  tui_point gbl_point_mapper(tui_point local_point) const;

  bool contains_global(tui_point point) const {
    return gbl_point_interpreter(point).is_in_local(area);
  }

  bool contains(tui_point point) const {
    return point.is_in(area);
  }

  bool contains_local(tui_point point) const {
    return point.is_in_local(area);
  }
  
  virtual tui_widget *proxy() const;
  tui_widget *proxy_penetrator() const;
  // tui_widget *operator -> () const {return proxy_penetrator();}
  tui_widget *unproxy_penetrator() const;
  tui_widget *get_window() const;
  bool is_hovers() const;

  bool is_focused() const {
    return unproxy_penetrator()->focused;
  }

  virtual void set_updated() {
    if (proxy()) {
      proxy_penetrator()->set_updated();
    } else {
      updated = true;
    }
  };
  virtual void reset_updated() {
    if (proxy()) {
      proxy_penetrator()->reset_updated();
    } else {
      updated = false;
    }
  };

  virtual bool get_updated() const {
    return proxy_penetrator()->updated;
  }

  tui_widget *get_parent() const {
    return unproxy_penetrator()->parent;
  }
  std::vector<tui_widget *> &get_children() const {
    return unproxy_penetrator()->children;
  }
  tui_widget *get_frame() const {
    return unproxy_penetrator()->frame;
  }

  tui_point translate_mouse_event(tui_mouse_event *event) const {
    return gbl_point_interpreter(event->get_point());
  }

  virtual tui_event *on_event(tui_event *event);
  virtual void draw(tui_point point) const;

  virtual void update();
  void update_hovers();

  virtual tui_event *on_child_exit(tui_widget *child);
  virtual void on_area_change(tui_rect new_area);
  virtual ~tui_widget();
};

extern tui_widget *root;
void tui_widget_init();

#endif