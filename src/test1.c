#include <stdio.h>
#include <unistd.h>
/*****************************/
#include "../../log/log.c"
/*****************************/
#include "menu-simple.c"
#include "menu.h"


/*****************************/


int main(int argc, char *argv[]) {
  struct MenuResult r = menu_simple();

  log_info("Result Code: %d", r.result);
  log_info("Selected: %d", r.selected);
  log_info("Command: %s", r.command);
  log_info("# Items: %d", r.items);
}

