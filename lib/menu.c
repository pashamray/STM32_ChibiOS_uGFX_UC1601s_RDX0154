/**
 * Simple menu
 */

#include "menu.h"

void menu_back(menu_state * ms)
{
	if(ms->current_menu->prev != NULL)
	{
		ms->current_menu = ms->current_menu->prev;		
		ms->top = ms->last_top;
		ms->current_item = ms->last_current_item;
	}
	if(ms->DRAW_AUTO) menu_draw(ms);
}

void menu_select(menu_state * ms, uint8_t draw)
{
	if(ms->current_menu->items[ms->current_item]->items != NULL)
	{
		ms->current_menu = ms->current_menu->items[ms->current_item];
		ms->last_top = ms->top;
		ms->last_current_item = ms->current_item;
		ms->top = 0;
		ms->current_item = 0;
	} else if(ms->current_menu->items[ms->current_item]->func != NULL) {
		menu_call(ms->current_menu->items[ms->current_item]->func);
	}
	if((ms->DRAW_AUTO) && draw) menu_draw(ms);
}

void menu_down(menu_state * ms)
{
	if(ms->top != ms->current_menu->count_items) {
		if(ms->current_item != (ms->current_menu->count_items-1)) {
			ms->current_item += 1;
			if(((ms->current_item) - (ms->top)) == ms->MAX_ROWS) {
				ms->top += 1;
			}
		}
	}
	if(ms->DRAW_AUTO) menu_draw(ms);
}

void menu_up(menu_state * ms)
{
	if(!((ms->top == 0) && (ms->current_item == 0))) {
		ms->current_item -= 1;
		if((ms->top > ms->current_item)) {
			ms->top -= 1;
		}
	}
	if(ms->DRAW_AUTO) menu_draw(ms);
}

void menu_draw(menu_state * ms)
{
	font_t font;
	coord_t width = gdispGetWidth();
	coord_t height = gdispGetHeight() / ms->MAX_ROWS;

	color_t color_text = White;
	color_t color_bg = Black;

	font = gdispOpenFont(ms->font);

	coord_t i;
	coord_t y = 0;

	gdispClear(Black);

	for (i = ms->top; i < ms->top + ms->MAX_ROWS; ++i) {
		if(i < ms->current_menu->count_items) {
			color_text = (ms->current_item == i) ? Black : White;
			color_bg   = (color_text == White)   ? Black : White;
			gdispFillStringBox(0,  (y * (height + 1)), width, height, ms->current_menu->items[i]->label, font, color_text, color_bg, justifyCenter);
		}
		y++;
	}

	gdispCloseFont(font);
}