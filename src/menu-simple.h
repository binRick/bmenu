
typedef struct MenuResult {
  int  selected;
  int  result;
  char *command;
  int  items;
} MenuResult;

struct MenuResult menu_simple();
