#include "ch.h"
#include "hal.h"

#include "stdlib.h"
#include "string.h"
#include "gfx.h"

#include "lib/menu.h"
/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /* Initialize and clear the display */
  gfxInit();

  menu_state mms;
  menu mm;
/*
  count_items
  label
  func
  prev
  items
*/

  menu mm_item01 = { "Пункт 01",  NULL, NULL, NULL, 0 };
  menu mm_item02 = { "Пункт 02",  NULL, NULL, NULL, 0 };
  menu mm_item03 = { "Пункт 03",  NULL, NULL, NULL, 0 };

  menu settings_mode   = { "Режим", NULL, NULL, NULL, 0 };
  menu settings_item02 = { "Тест", NULL, NULL, NULL, 0 };
  menu settings_item03 = { "Пункт 01_03", NULL, NULL, NULL, 0 };
  menu settings_item04 = { "Пункт 01_04", NULL, NULL, NULL, 0 };

  menu lang_english = { "English", NULL, NULL, NULL, 0 };
  menu lang_russian = { "Русский", NULL, NULL, NULL, 0 };

  menu * lang_items[] = {
                                &lang_english,
                                &lang_russian
                              };

  menu settings_lang = { "Язык", NULL, NULL, lang_items, 0 };

  menu * settings_items[] = { 
                                    &settings_mode,
                                    &settings_item02, 
                                    &settings_item03, 
                                    &settings_item04, 
                                    &settings_lang
                                  };

  menu mm_settings = { "Настройки", NULL, &mm, settings_items, 5 };

  menu * mm_items[] = { &mm_item01,
                        &mm_item02, 
                        &mm_item03, 
                        &mm_settings }; 

  mm.count_items = 4;
  mm.label = "menu";
  mm.func = NULL;
  mm.prev = NULL;
  mm.items = mm_items;

  mms.MAX_ROWS = 4;
  mms.DRAW_AUTO = TRUE;
  mms.font = "Archangelsk Regular 12";
  mms.top = 0;
  mms.last_top = 0;
  mms.current_item = 0;
  mms.last_current_item = 0;
  mms.current_menu = &mm;

  menu_draw(&mms);
  gfxSleepMilliseconds(2000);
  menu_down(&mms);
  gfxSleepMilliseconds(2000);
  menu_down(&mms);
  gfxSleepMilliseconds(2000);
  menu_down(&mms);
  gfxSleepMilliseconds(2000);
  menu_select(&mms, TRUE);
  gfxSleepMilliseconds(2000);
  menu_down(&mms);
  gfxSleepMilliseconds(2000);
  //menu_select(&mms, FALSE);
  //gfxSleepMilliseconds(5000);
  menu_draw(&mms);

  // Wait forever
  while(TRUE) {
    gfxSleepMilliseconds(500);
  }
}