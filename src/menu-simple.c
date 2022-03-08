#include "../../log/log.c"
#include "menu.h"
#include <stdio.h>
#include <unistd.h>


int menu_simple(){
  int c, lo = 1, fo = 1;
  int menu_result = menu_load();

  log_info("%d items> res:%d", menu_get_count(), menu_result);

  switch (menu_result) {
  case 1:
    fprintf(stderr, "Please set HOME environment variable.\n");
    return(1);

    break;
  case 2:
    fprintf(stderr, "Could not open config file: %s\n", menu_get_config_path());
    return(2);

    break;
  case 3:
    fprintf(stderr, "Memory allocation error. Could not open config file: %s\n", menu_get_config_path());
    return(3);

    break;
  case 4:
    fprintf(stderr, "Invalid line format detected in config file: %s.\n", menu_get_config_path());
    return(4);

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
  switch (fo) {
  case 1:
    log_info("menu ended. selected option #%d", lo);
    log_info("      cmd: %s", get_command(lo));
    break;
  case 2:
    log_info("menu cancelled");
    break;
  default:
    log_error("unhandled menu result %d", fo);
    return(10);

    break;
  }

  fflush(stdout);
  menu_free_all();

  return(0);
} /* menu_simple */
