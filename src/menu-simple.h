#ifndef SIMPLE_MENU_H
#define SIMPLE_MENU_H
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_ERROR
#endif
#include "reflect/src/reflect.c"
#include "reflect/include/stp_print.h"
/*********************************/
#include "reflect/src/stp_print.c"

typedef struct MenuResult1 {
  int  selected;
  int  result;
  char *command;
  int  items;
  bool ok;
  char *dur;
  unsigned long ms;
  unsigned int started;
  unsigned int ended;
} MenuResult1;


/*********************************************/
REFLECTIVE(MenuResult2,
           FIELD(int, selected);
           FIELD(int, result);
           FIELD(char *, command);
           FIELD(int, items);
           FIELD(bool, ok);
           FIELD(char *, dur);
           FIELD(int, ms);
           FIELD(unsigned long, started);
           FIELD(unsigned long, ended);
           );

stp_print_begin(MenuResult2, 9) {
  stp_print_field_int(selected);
  stp_print_field_int(result);
  stp_print_field_string(command);
  stp_print_field_int(items);
  stp_print_field_int(ok);
  stp_print_field_string(dur);
  stp_print_field_int(ms);
  stp_print_field_uint(started);
  stp_print_field_uint(ended);
  stp_print_end();
}
/*********************************************/

MenuResult1 * menu_simple();

#endif
