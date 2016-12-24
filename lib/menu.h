/**
 * Simple menu
 */

#ifndef _MENU_H_
#define _MENU_H_

#include "gfx.h"

#define menu_call(fp) ((*fp)())

typedef void (*menu_func)(void);

typedef struct menu
{
	const char * label;
	menu_func func;
	struct menu * prev;
	struct menu ** items;
	uint8_t count_items;
} menu;

typedef struct menu_state
{	
	uint8_t MAX_ROWS;
	uint8_t DRAW_AUTO;
	const char * font;
	uint8_t top;
	uint8_t last_top;
	uint8_t current_item;
	uint8_t last_current_item;
	struct menu * current_menu;
} menu_state;

void menu_back(menu_state * ms);
void menu_select(menu_state * ms, uint8_t draw);
void menu_up(menu_state * ms);
void menu_down(menu_state * ms);
void menu_draw(menu_state * ms);

#endif