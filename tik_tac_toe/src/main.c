/* src/main.c
 * Entry point for Tic‑Tac‑Toe on Zephyr + LVGL.
 */

 #include <zephyr/device.h>
 #include <zephyr/drivers/display.h>
 #include <zephyr/kernel.h>
 #include <lvgl.h>
 #include "config.h"
 #include "ui.h"
 #include "input.h"
 
 /** LVGL screen object, shared with input.c */
 lv_obj_t *scr;
 
 int main(void)
 {
     const struct device *disp = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
     if (!device_is_ready(disp)) {
         printk("Display device not ready\n");
         return -1;
     }
     display_blanking_off(disp);
 
     /* Allow LVGL to initialize its timers */
     lv_timer_handler();
 
     scr = lv_scr_act();
     create_board_ui(scr);
 
     while (1) {
         lv_timer_handler();
         k_sleep(K_MSEC(10));
     }

     return 0;
 }
 