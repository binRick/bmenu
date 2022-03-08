#include "../../log/log.c"
#include "menu-simple.h"
#include "menu.h"
#include <stdio.h>
#include <unistd.h>

struct MenuResult menu_simple(){
  struct MenuResult *R = malloc(sizeof(MenuResult));

  sprintf(&R->command, "%s", "UNKNOWN");
  struct MenuResult r = *R;
  int               c, lo = 1, fo = 1;

  r.result = menu_load();
  r.items  = menu_get_count();

  log_info("%d items> res:%d", menu_get_count(), r.result);

  switch (r.result) {
  case 1:
    fprintf(stderr, "Please set HOME environment variable.\n");
    return(r);

    break;
  case 2:
    r.result = 2;
    fprintf(stderr, "Could not open config file: %s\n", menu_get_config_path());
    return(r);

    break;
  case 3:
    r.result = 3;
    fprintf(stderr, "Memory allocation error. Could not open config file: %s\n", menu_get_config_path());
    return(r);

    break;
  case 4:
    r.result = 4;
    fprintf(stderr, "Invalid line format detected in config file: %s.\n", menu_get_config_path());
    return(r);

    break;
  }
  menu_init();
  menu_show(VERSION, lo, fo);
  while ((c = getchar()) != KEY_ENTER) {
    if (c == KEY_ESC && getchar() == 91) {
      c = getchar();
    }
    switch (c) {
    case KEY_UP:
    case KEY_K:
      if (lo > 1) {
        --lo;
      }
      break;
    case KEY_DOWN:
    case KEY_SPACE:
    case KEY_J:
      if (lo < menu_get_count()) {
        ++lo;
      }
      break;
    case KEY_TAB:
      switch (fo) {
      case 1:
        fo = 2;
        break;
      case 2:
        fo = 1;
        break;
      }
      break;

    case KEY_RIGHT:
    case KEY_L:
      fo = 2;
      break;
    case KEY_LEFT:
    case KEY_H:
      fo = 1;
      break;
    default:
      log_debug("Unhandled key: %d", c);

      break;
    }
    menu_show(VERSION, lo, fo);
  }
  menu_end();
  r.selected = lo;

  r.result = fo - 1;
  switch (fo) {
  case 1:
    log_info("menu ended. selected option #%d", lo);
    log_info("      cmd: %s", get_command(lo));
    r.command = strdup(get_command(lo));
    break;
  case 2:
    log_info("menu cancelled");
    break;
  default:
    log_error("unhandled menu result %d", fo);

    break;
  }

  fflush(stdout);
  menu_free_all();

  return(r);
} /* menu_simple */
