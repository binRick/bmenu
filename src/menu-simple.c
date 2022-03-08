#include "log/log.c"
#include "menu-simple.h"
#include "menu.h"
#include "time/timequick.h"
#include "time/timestamp.c"
#include <stdio.h>
#include <unistd.h>


MenuResult1 * new_menu_result(){
  MenuResult1 *t = malloc(sizeof(MenuResult1));

  t->started = timestamp();
  t->ok      = false;
  sprintf(&t->command, "%s", "UNKNOWN");
  t->items = menu_get_count();
  return(t);
}


MenuResult1 * menu_simple(){
  log_set_level(LOG_LEVEL);
  MenuResult1 *R = new_menu_result();
  int         c, lo = 1, fo = 1;

  tq_start("menu load");
  R->result = menu_load();
  R->dur    = strdup(tq_stop("menu load"));
  R->ended  = timestamp();
  R->ms     = R->ended - R->started;

  log_info("%d items> res:%d in %s", R->items, R->result, R->dur);

  switch (R->result) {
  case 0:
    log_debug(
      AC_RESETALL AC_GREEN AC_REVERSED "#%d Items loaded!" AC_RESETALL
      "",
      menu_get_count()
      );

    break;
  case 1:
    fprintf(stderr, "Please set HOME environment variable.\n");
    return(R);

    break;
  case 2:
    R->result = 2;
    fprintf(stderr, "Could not open config file: %s\n", menu_get_config_path());
    return(R);

    break;
  case 3:
    R->result = 3;
    fprintf(stderr, "Memory allocation error. Could not open config file: %s\n", menu_get_config_path());
    return(R);

    break;
  case 4:
    R->result = 4;
    fprintf(stderr, "Invalid line format detected in config file: %s.\n", menu_get_config_path());
    return(R);

    break;
  }
  menu_init();
  menu_show(VERSION, lo, fo);
  while ((c = getchar()) != KEY_ENTER) {
    if (c == KEY_ESC && getchar() == 91) {
      c = getchar();
    }
    switch (c) {
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
      lo = c - 48;
      break;
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
    } /* switch */
    menu_show(VERSION, lo, fo);
  }
  menu_end();
  R->selected = lo;

  R->result = (fo - 1);
  switch (fo) {
  case 1:
    R->ok      = true;
    R->command = strdup(get_command(lo));
    log_info("menu ended. selected option #%d", lo);
    log_info("      cmd: %s", R->command);
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

  return(R);
} /* menu_simple */
