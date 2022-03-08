#ifndef MENU_H
#define MENU_H               1

#define DEFAULT_LOG_LEVEL    3


// Macros
#define UNUSED(x)    (void)(x)

void menu_init(void);
void menu_end(void);
void menu_set_title(char *);
void menu_set_config(char *);
int  menu_load(void);
int  menu_get_count(void);
char *menu_get_config_path(void);
void menu_show(char *, int, int);
void menu_execute(int);
void menu_free_all(void);


#define KEY_ENTER    10       // enter key
#define KEY_ESC      27       // escape char indicating an arrow key
#define KEY_DOWN     66       // down-arrow key
#define KEY_UP       65       // up-arrow key
#define KEY_LEFT     68       // left-arrow key
#define KEY_RIGHT    67       // right-arrow key
#define KEY_H        104      // h key
#define KEY_J        106      // j key
#define KEY_K        107      // k key
#define KEY_L        108      // l key
#define KEY_TAB      9        // tab key
#define KEY_SPACE    32       // space key

#ifndef VERSION
#define VERSION      "0.2.1"
#endif

#endif
