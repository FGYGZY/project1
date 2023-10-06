#include <minitui.h>
#include <basics.h>
#include <ansi.h>

const char tui_msgbox::hint[] = "[Y]es, (N)o, (C)ancel";

// @param event triggered event
// @return event for parents
tui_event *
tui_msgbox::on_event(
  tui_event *event
) {
  if (!event) {
    Warn("A NULL event given!");
  }
  if (event->event_type == KEYBD_EVENT) {
    if (event->event_body) {
      Warn("An empty event given!");
    }
    auto kbd_event = (tui_kbd_event *) event->event_body;
    char code = kbd_event->code;
    if (code >= 'A' && code <= 'Z') code += 'a' - 'A';
    switch (code)
    {
    case 'y': case 'n':  case 'c': {
      tui_exit_event *exit_event = (tui_exit_event *) malloc(sizeof(tui_exit_event));
      exit_event->retcode = code;
      if (event->event_body)
        free(event->event_body);
      event->event_type = EXIT_EVENT;
      event->event_body = exit_event;
      return event;
    }
    default:
      break;
    }
  }
  if (event->event_body)
    free(event->event_body);
  free(event);
  return NULL;
}

// @param point location relative to up-left corner
void
tui_msgbox::draw(
  tui_point point
) const {
  ANSI_CMD(ANSI_RST);
  if (point.x == area.height() - 1) {
    if (point.y < sizeof(hint) - 1) {
      putchar(hint[point.y]);
    } else {
      putchar(' ');
    }
  } else {
    int pos = point.x * area.width() + point.y;
    if (pos < msg_length)
      putchar(message[pos]);
    else putchar(' ');
  }
  ANSI_CMD(ANSI_RST);
}