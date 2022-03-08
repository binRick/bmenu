#include <stdio.h>
#include <unistd.h>
/*****************************/
#include "menu-simple.h"
/*****************************/
#include "log/log.c"
/*****************************/
#include "menu.h"
/*****************************/
#include "menu-simple.c"
/*****************************/


int main(int argc, char *argv[]) {
  MenuResult1 *r = menu_simple();

  log_info("Result Code: %d", r->result);
  log_info("Selected: %d", r->selected);
  log_info("Command: %s", r->command);
  log_info("# Items: %d", r->items);
  fprintf(stdout,"%s\n",r->command);
}

