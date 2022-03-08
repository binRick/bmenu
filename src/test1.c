#define LOG_LEVEL    LOG_TRACE
/*****************************/
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
//#include "string/ring_buffer.h"
#include "string/stringbuffer.c"
#include "string/stringbuffer.h"
//#include "string/stringfn.h"
#include "ic/ic.h"
#include "time/timequick.h"
#include "time/timestamp.c"
#include "uuid/uuid4.h"


/*********************************/


//#include "reflect/src/stp_print.c"


int main(int argc, char *argv[]) {
  MenuResult1 *r = menu_simple();

  log_info("Result Code: %d", r->result);
  log_info("Selected: %d", r->selected);
  log_info("Command: %s", r->command);
  log_info("# Items: %d", r->items);
  log_info("ms: %d", r->ms);
  log_info("started: %lu", r->started);
  log_info("Ended: %lu", r->ended);
  log_info("Duration: %s", r->dur);

  ic(r->dur, r->command);
  fprintf(stdout, "%s\n", r->command);
}

