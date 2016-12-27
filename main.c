#include "ch.h"
#include "hal.h"

#include "stdlib.h"
#include "string.h"
#include "gfx.h"

#include "lib/menu.h"


#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000

volatile uint32_t *DWT_CTRL = (uint32_t *) 0xE0001000;

#define SCB_DEMCR (*(volatile unsigned *)0xE000EDFC)
#define CPU_RESET_CYCLECOUNTER do {  } while(0)

static int uitoa(unsigned int value, char * buf, int max) {
    int n = 0;
    int i = 0;
    unsigned int tmp = 0;

    if (!buf)
        return -3;

    if (2 > max)
        return -4;

    i=1;
    tmp = value;
    if (0 > tmp) {
        tmp *= -1;
        i++;
    }
    for (;;) {
        tmp /= 10;
        if (0 >= tmp)
            break;
        i++;
    }
    if (i >= max) {
        buf[0] = '?';
        buf[1] = 0x0;
        return 2;
    }

    n = i;
    tmp = value;
    if (0 > tmp) {
        tmp *= -1;
    }
    buf[i--] = 0x0;
    for (;;) {
        buf[i--] = (tmp % 10) + '0';
        tmp /= 10;
        if (0 >= tmp) {
            break;
        }
    }
    if (-1 != i) {
        buf[i--] = '-';
    }

    return n;
}

void benchmark(void) {
    uint32_t i, pixels, ms, pps;
    char pps_str[25];
  coord_t height, width, rx, ry, rcx, rcy;
    color_t random_color;
  font_t font;

  width = gdispGetWidth();
  height = gdispGetHeight();
    font = gdispOpenFont("UI2 Double");

   gdispClear(Black);

  gdispDrawStringBox(0, 0, width, 20, "ChibiOS/GFX", font, White, justifyCenter);
  gdispDrawStringBox(0, 21, width, 20, "Benchmark", font, White, justifyCenter);

  font = gdispOpenFont("UI2");
  gdispDrawStringBox(0, 43, width, 20, "5000 random", font, White, justifyCenter);
  gdispDrawStringBox(0, 63, width, 20, "rectangles", font, White, justifyCenter);
  
  gfxSleepMilliseconds(3000);
  
  /* seed for the rand() */
  srand(DWT_CYCCNT);
  pixels = 0;

  CPU_RESET_CYCLECOUNTER;

  for (i = 0; i < 5000; i++) {
    random_color = (rand() % 2);
    rx = (rand() % (width-10));
    ry = (rand() % (height-10));
    rcx = (rand() % ((width-rx)-10))+10;
    rcy = (rand() % ((height-ry)-10))+10;

    gdispFillArea(rx, ry, rcx, rcy, random_color);
    pixels += (rcx+1)*(rcy+1);
  }

  ms = DWT_CYCCNT / 168000;
  pps = (float)pixels/((float)ms/1000.0f);

  memset (pps_str, 0, sizeof(pps_str));
  uitoa(pps, pps_str, sizeof(pps_str));
  strcat(pps_str, " px/s");

  font = gdispOpenFont("UI2 Double");
  gdispClear(Black);
  gdispDrawStringBox(0, 0, width, 20, "ChibiOS/GFX", font, White, justifyCenter);
  gdispDrawStringBox(0, 21, width, 20, "Benchmark", font, White, justifyCenter);

  gdispDrawStringBox(0, 42, width, 20, pps_str, font, White, justifyCenter);
  //gdispDrawString(20, height/2, pps_str, font, White);
}

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
  gfxSleepMilliseconds(1000);
  menu_down(&mms);
  gfxSleepMilliseconds(1000);
  menu_down(&mms);
  gfxSleepMilliseconds(1000);
  menu_down(&mms);
  gfxSleepMilliseconds(1000);
  menu_select(&mms, TRUE);
  gfxSleepMilliseconds(1000);
  menu_down(&mms);
  gfxSleepMilliseconds(1000);
  menu_select(&mms, FALSE);
  gfxSleepMilliseconds(1000);
  menu_draw(&mms);

  // Wait forever

  while(TRUE) {
    gfxSleepMilliseconds(1000);
  }
}