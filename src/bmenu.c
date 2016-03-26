// Copyright (c) 2016 Brian Barto
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 of the License, or (at your option)
// any later version.  See COPYING for more details.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>          // getenv()
#include <sys/ioctl.h>       // Support for terminal dimentions
#include <termios.h>         // Support for character input
#include <unistd.h>          // execl()

#define VERSION              "0.1.0"

#define MENU_CONFIG          ".bmenu"

#define MAX_MENU_OPTIONS     10
#define MAX_MENU_CHAR        50
#define MAX_COMMAND_CHAR     100

#define SPACE                040
#define NEWLINE              012
#define ENTER                012

// Hex codes for box chars according to:
// http://www.utf8-chartable.de/unicode-utf8-table.pl?start=9472&unicodeinhtml=dec
#define BOX_LINE_HORIZONTAL          {0xe2, 0x94, 0x80, '\0'}
#define BOX_LINE_HORIZONTAL_H        {0xe2, 0x94, 0x81, '\0'}
#define BOX_LINE_HORIZONTAL_D        {0xe2, 0x95, 0x90, '\0'}
#define BOX_LINE_VERTICAL            {0xe2, 0x94, 0x82, '\0'}
#define BOX_LINE_VERTICAL_H          {0xe2, 0x94, 0x83, '\0'}
#define BOX_LINE_VERTICAL_D          {0xe2, 0x95, 0x91, '\0'}
#define BOX_CORNER_LEFTTOP           {0xe2, 0x94, 0x8c, '\0'}
#define BOX_CORNER_LEFTTOP_H         {0xe2, 0x94, 0x8f, '\0'}
#define BOX_CORNER_LEFTTOP_D         {0xe2, 0x95, 0x94, '\0'}
#define BOX_CORNER_RIGHTTOP          {0xe2, 0x94, 0x90, '\0'}
#define BOX_CORNER_RIGHTTOP_HL       {0xe2, 0x94, 0x91, '\0'}
#define BOX_CORNER_RIGHTTOP_DL       {0xe2, 0x95, 0x95, '\0'}
#define BOX_CORNER_RIGHTTOP_HD       {0xe2, 0x94, 0x92, '\0'}
#define BOX_CORNER_RIGHTTOP_DD       {0xe2, 0x95, 0x96, '\0'}
#define BOX_CORNER_RIGHTTOP_H        {0xe2, 0x94, 0x93, '\0'}
#define BOX_CORNER_RIGHTTOP_D        {0xe2, 0x95, 0x97, '\0'}
#define BOX_CORNER_LEFTBOTTOM        {0xe2, 0x94, 0x94, '\0'}
#define BOX_CORNER_LEFTBOTTOM_HR     {0xe2, 0x94, 0x95, '\0'}
#define BOX_CORNER_LEFTBOTTOM_DR     {0xe2, 0x95, 0x98, '\0'}
#define BOX_CORNER_LEFTBOTTOM_HU     {0xe2, 0x94, 0x96, '\0'}
#define BOX_CORNER_LEFTBOTTOM_DU     {0xe2, 0x95, 0x99, '\0'}
#define BOX_CORNER_LEFTBOTTOM_H      {0xe2, 0x94, 0x97, '\0'}
#define BOX_CORNER_LEFTBOTTOM_D      {0xe2, 0x95, 0x9a, '\0'}
#define BOX_CORNER_RIGHTBOTTOM       {0xe2, 0x94, 0x98, '\0'}
#define BOX_CORNER_RIGHTBOTTOM_H     {0xe2, 0x94, 0x9b, '\0'}
#define BOX_CORNER_RIGHTBOTTOM_D     {0xe2, 0x95, 0x9d, '\0'}
#define BOX_CROSS_LEFT               {0xe2, 0x94, 0x9c, '\0'}
#define BOX_CROSS_LEFT_HD            {0xe2, 0x94, 0xa0, '\0'}
#define BOX_CROSS_LEFT_DD            {0xe2, 0x95, 0x9f, '\0'}
#define BOX_CROSS_LEFT_H             {0xe2, 0x94, 0xa3, '\0'}
#define BOX_CROSS_LEFT_D             {0xe2, 0x95, 0xa0, '\0'}
#define BOX_CROSS_RIGHT              {0xe2, 0x94, 0xa4, '\0'}
#define BOX_CROSS_RIGHT_H            {0xe2, 0x94, 0xab, '\0'}
#define BOX_CROSS_RIGHT_D            {0xe2, 0x95, 0xa3, '\0'}
#define BOX_CROSS_TOP                {0xe2, 0x94, 0xac, '\0'}
#define BOX_CROSS_TOP_H              {0xe2, 0x94, 0xb3, '\0'}
#define BOX_CROSS_TOP_D              {0xe2, 0x95, 0xa6, '\0'}
#define BOX_CROSS_BOTTOM             {0xe2, 0x94, 0xb4, '\0'}
#define BOX_CROSS_BOTTOM_H           {0xe2, 0x94, 0xbb, '\0'}
#define BOX_CROSS_BOTTOM_D           {0xe2, 0x95, 0xa9, '\0'}
#define BOX_CROSS_MIDDLE             {0xe2, 0x94, 0xbc, '\0'}
#define BOX_CROSS_MIDDLE_H           {0xe2, 0x94, 0x8b, '\0'}
#define BOX_CROSS_MIDDLE_D           {0xe2, 0x95, 0xac, '\0'}
#define SHADE_MEDIUM                 {0xe2, 0x96, 0x92, '\0'}
#define POINTER_RIGHT                {0xe2, 0x96, 0xb6, '\0'}

#define KNRM                 "\x1B[0m"
#define KRED                 "\x1B[31m"
#define KGRN                 "\x1B[32m"
#define KYEL                 "\x1B[33m"
#define KBLU                 "\x1B[34m"
#define KMAG                 "\x1B[35m"
#define KMAG_BOLD            "\x1B[35;1m"
#define KCYN                 "\x1B[36m"
#define KCYN_BOLD            "\x1B[36;1m"
#define KWHT                 "\x1B[37m"

// Global Vars
int windowRows, windowCols;
char menu[MAX_MENU_OPTIONS][MAX_MENU_CHAR];
char command[MAX_MENU_OPTIONS][MAX_COMMAND_CHAR];

// Function prototypes
int getMenuRows(void);
int getMenuCols(void);

/***************************************************
 * Main function
 *
 * Returns non-zero value on error. Check stderr for
 * more info.
 ***************************************************/
int main (void) {
	int loadMenuConfig(void);
	void windowHeader(void);
	void decorateMenu(void);
	void printMenu(int, int);

	// Initialize menu and command arrays
	for (int row = 0; row < MAX_MENU_OPTIONS; ++row) {
		for (int col = 0; col < MAX_MENU_CHAR; ++col)
			menu[row][col] = '\0';
		for (int col = 0; col < MAX_COMMAND_CHAR; ++col)
			command[row][col] = '\0';
	}

	// Getting menu config
	int result = loadMenuConfig();
	if (result == 1) {
		fprintf(stderr, "Please set HOME environment variable.\n");
		return result;
	} else if (result == 2) {
		fprintf(stderr, "Could not open config file: ~/" MENU_CONFIG "\n");
		return result;
	}

	// Get terminal window size
	struct winsize w;                                                                                         
	ioctl(0, TIOCGWINSZ, &w);
	windowRows =  w.ws_row;
	windowCols = w.ws_col;

	// initializing terminal window with all spaces
	for (int c1 = 0; c1 < windowRows; ++c1) {
		int c2;
		for (c2 = 0; c2 < windowCols - 1; ++c2) {
			printf("\033[%i;%iH", c1, c2);
			printf("%c", SPACE);
		}
		printf("\033[%i;%iH", c1, c2);
		printf("%c", NEWLINE);
	}

	// Print window header
	windowHeader();

	// Menu title and borders
	decorateMenu();

	// Setting terminal input mode to turn off echo and buffering
	static struct termios oldt, newt;
	tcgetattr( STDIN_FILENO, &oldt);            // get terminal parameters, store in oldt
	newt = oldt;                                // copy settings to newt
	newt.c_lflag &= ~(ICANON | ECHO);           // unset ICANON and ECHO
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);   // run new terminal settings

	// Menu loop
	int menuListOption = 1, menuFootOption = 1;
	int input = 0;
	int menuRows = getMenuRows();
	do {
		// Check input
		if (input == 27) {
			input = getchar();
			input = getchar();
			if (input == 65 && menuListOption > 1)
				--menuListOption;
			else if (input == 66 && menuListOption < menuRows)
				++menuListOption;
			else if (input == 68)
				menuFootOption = 1;
			else if (input == 67)
				menuFootOption = 2;
		}

		// Print menu with the current selection highlighted
		printMenu(menuListOption, menuFootOption);

		// Position cursor at the bottom of the terminal window
		printf(KNRM);
		printf("\033[%i;%iH", windowRows, 1);

	} while ((input = getchar()) != ENTER);

	// Restore terminal settings before exiting
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

	// Execute chosen command
	if (menuFootOption == 1)
		execl("/bin/sh", "/bin/sh", "-c", command[menuListOption - 1], 0);

	return 0;
}

/***************************************************
 * loadMenuConfig()
 *
 * Loading the menu config file. Return a non-zero
 * result if anything goes wrong.
 ***************************************************/
int loadMenuConfig(void) {
	char *homeDir = getenv("HOME");;

	if (homeDir == NULL) {
		return 1;
	}

	int len = strlen(homeDir) + strlen("/" MENU_CONFIG);
	char menuConfigPath[len + 1];
	strcpy(menuConfigPath, homeDir);
	strcat(menuConfigPath, "/");
	strcat(menuConfigPath, MENU_CONFIG);
	FILE *menuConfig;
	if ((menuConfig = fopen(menuConfigPath, "r")) == NULL) {
		return 2;
	}

	_Bool menuOn = 1, commandOn = 0;
	int l = 0, i = 0, c;
	while((c = getc(menuConfig)) != EOF) {
		if (menuOn && i == MAX_MENU_CHAR - 1) {
			menuOn = 0;
			continue;
		}
		if (commandOn && i == MAX_COMMAND_CHAR - 1) {
			commandOn = 0;
			continue;
		}
			
		if (c == ':') {
			commandOn = 1;
			menuOn = 0;
			i = 0;
			continue;
		} else if (c == '\n') {
			menuOn = 1;
			commandOn = 0;
			i = 0;
			++l;
			if (l > MAX_MENU_OPTIONS - 1)
				break;
			else {
				menu[l][0] = '\0';
				command[l][0] = '\0';
			}
			continue;
		}

		if (menuOn) {
			menu[l][i] = c;
			menu[l][i+1] = '\0';
		} else if (commandOn) {
			command[l][i] = c;
			command[l][i+1] = '\0';
		}

		++i;
	}
	
	fclose(menuConfig);

	return 0;
}

/*************************************************
 * windowHeader()
 *
 * Prints the window header (title and crossbar).
 *************************************************/
void windowHeader(void) {
	int textRow = 1;
	int barRow = 2;

	printf(KCYN_BOLD);
	printf("\033[%i;%iH%s", textRow, 2, "B-MENU v" VERSION);

	for (int col = 0; col < windowCols; ++col) {
		char c[] = BOX_LINE_HORIZONTAL_D;
		printf("\033[%i;%iH%s", barRow, col, c);
	}
}

/*************************************************
 * decorateMenu()
 *
 * Prints the inner and outer borders for the menu.
 *************************************************/
void decorateMenu() {
	int borderCols, borderRows, startRow, startCol;

	// Border size (inner)
	borderCols = getMenuCols() + 8;
	borderRows = getMenuRows() + 4;

	// Minimum border width is 25 cols.
	// Need at least this much for select/exit options.
	if (borderCols < 25)
		borderCols = 25;

	// Determining starting row and column for border (inner)
	startCol = ((windowCols / 2) - (borderCols / 2));
	startRow = ((windowRows / 2) - (borderRows / 2));
	if (startCol < 0)
		startCol = 0;
	if (startRow < 0)
		startRow = 0;

	// printing border (inner)
	for (int row = 0; row < borderRows; ++row)
		for (int col = 0; col < borderCols; ++col)
			if (row == 0 && col == 0) {
				char c[] = BOX_CORNER_LEFTTOP;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == 0 && col == borderCols - 1) {
				char c[] = BOX_CORNER_RIGHTTOP_DD;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == 0) {
				char c[] = BOX_LINE_HORIZONTAL;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == borderRows - 1 && col == 0) {
				char c[] = BOX_CORNER_LEFTBOTTOM_DR;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == borderRows - 1 && col == borderCols - 1) {
				char c[] = BOX_CORNER_RIGHTBOTTOM_D;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == borderRows - 1) {
				char c[] = BOX_LINE_HORIZONTAL_D;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (col == 0) {
				char c[] = BOX_LINE_VERTICAL;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (col == borderCols - 1) {
				char c[] = BOX_LINE_VERTICAL_D;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			}

	// Printing (inner) border title
	printf(KCYN_BOLD);
	printf("\033[%i;%iH%s", startRow - 1, startCol, "Select Option");

	// Border size (outer)
	borderCols += 4;
	borderRows += 4;

	// Determining starting row and column for border (outer)
	startCol = ((windowCols / 2) - (borderCols / 2));
	startRow = ((windowRows / 2) - (borderRows / 2));
	if (startCol < 0)
		startCol = 0;
	if (startRow < 0)
		startRow = 0;

	// Add to the bottom of the border (outer) for select/exit options
	borderRows += 3;

	// printing border (outer)
	for (int row = 0; row < borderRows; ++row)
		for (int col = 0; col < borderCols; ++col)
			if (row == 0 && col == 0) {
				char c[] = BOX_CORNER_LEFTTOP_D;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == 0 && col == borderCols - 1) {
				char c[] = BOX_CORNER_RIGHTTOP_DL;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == 0) {
				char c[] = BOX_LINE_HORIZONTAL_D;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == borderRows - 1 && col == 0) {
				char c[] = BOX_CORNER_LEFTBOTTOM_DU;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (row == borderRows - 1 && col == borderCols - 1) {
				char c[] = BOX_CORNER_RIGHTBOTTOM;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
				char c2[] = SHADE_MEDIUM;
				printf("\033[%i;%iH%s", row + startRow + 1, col + startCol + 1, c2);
			} else if (row == borderRows - 1) {
				char c[] = BOX_LINE_HORIZONTAL;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
				char c2[] = SHADE_MEDIUM;
				printf("\033[%i;%iH%s", row + startRow + 1, col + startCol + 1, c2);
			} else if (col == 0 && row == borderRows - 3) {
				char c[] = BOX_CROSS_LEFT_DD;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (col == borderCols - 1 && row == borderRows - 3) {
				char c[] = BOX_CROSS_RIGHT;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
				char c2[] = SHADE_MEDIUM;
				printf("\033[%i;%iH%s", row + startRow + 1, col + startCol + 1, c2);
			} else if (col == 0) {
				char c[] = BOX_LINE_VERTICAL_D;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			} else if (col == borderCols - 1) {
				char c[] = BOX_LINE_VERTICAL;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
				char c2[] = SHADE_MEDIUM;
				printf("\033[%i;%iH%s", row + startRow + 1, col + startCol + 1, c2);
			} else if (row == borderRows - 3) {
				char c[] = BOX_LINE_HORIZONTAL;
				printf("\033[%i;%iH%s", row + startRow, col + startCol, c);
			}
	
}

/*************************************************
 * printMenu()
 *
 * Prints the menu options list and the select/exit
 * options. Also highlights the current selected
 * options. 
 * 
 * Args:
 * int lo - List Option (currently selected)
 * int fo - Foot Option (currently selected)
 *************************************************/
void printMenu(int lo, int fo) {
	int startRow, startCol;
	int menuRows = getMenuRows();
	int menuCols = getMenuCols();

	// Determining starting row and column for menu
	startCol = ((windowCols / 2) - (menuCols / 2)) > 0 ? ((windowCols / 2) - (menuCols / 2)) : 0;
	startRow = ((windowRows / 2) - (menuRows / 2)) > 0 ? ((windowRows / 2) - (menuRows / 2)) : 0;

	// Inserting menu in to terminal window
	for (int row = 0; row < menuRows; ++row) {

		// highlighting current selection text
		printf( (row == lo - 1) ? KMAG_BOLD : KNRM KCYN );

		for (int col = 0; menu[row][col] != '\0'; ++col) {

			// Printing selection marker if on selected row, and removing any previous
			// marker if not.
			if (row == lo - 1 && col == 0) {
				char c[] = POINTER_RIGHT;
				printf("\033[%i;%iH%s", row + startRow, col + startCol - 2, c);
			} else if (col == 0)
				printf("\033[%i;%iH%c", row + startRow, col + startCol - 2, SPACE);

			// printing menu text
			printf("\033[%i;%iH%c", row + startRow, col + startCol, menu[row][col]);
		}

		// printing menu foot options (select/exit)
		if (row == menuRows - 1) {
			int sRow = row + startRow + 6;
			int sCol = (windowCols / 2) - ((menuCols + 8 > 25 ? menuCols + 8 : 25) / 2) + 1;
			int eCol = (windowCols / 2) + ((menuCols + 8 > 25 ? menuCols + 8 : 25) / 2) - 8;
			printf( (fo == 1) ? KMAG_BOLD : KNRM KCYN );
			printf("\033[%i;%iH%s", sRow, sCol, "< select >");
			printf( (fo == 2) ? KMAG_BOLD : KNRM KCYN );
			printf("\033[%i;%iH%s", sRow, eCol, "< exit >");
		}
	}
}

/*************************************************
 * getMenuRows()
 *
 * Gets the number of menu options (i.e. rows)
 * from the global menu[][] array. This defines
 * the height of the menu, and is needed for
 * centering and drawing borders.
 *
 * Return:
 * int - Number of menu rows
 *************************************************/
int getMenuRows(void) {
	int rows;

	for (rows = 0; menu[rows][0] != '\0' && rows < MAX_MENU_OPTIONS; ++rows)
		;

	return rows;
}

/*************************************************
 * getMenuCols()
 *
 * Gets the number of characters (i.e. cols) in
 * the longest menu option from the global menu[][]
 * array. This defines the width of the menu, and
 * is needed for centering and drawing borders.
 *
 * Return:
 * int - Number of characters in longest option
 *************************************************/
int getMenuCols(void) {
	int maxCols = 0;

	for (int rows = 0; menu[rows][0] != '\0' && rows < MAX_MENU_OPTIONS; ++rows)
		for (int cols = 1; menu[rows][cols] != '\0' && cols < MAX_MENU_CHAR; ++cols)
			if (maxCols < cols)
				maxCols = cols;

	return maxCols + 1;
}
