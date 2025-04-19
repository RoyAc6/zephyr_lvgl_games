/*
 * src/main.c - 2048 game on Zephyr RTOS for lcd_par_s035_8080 display
 * with LVGL grid, swipe/buttons input, animation, and full game logic
 *
 * Ensure in prj.conf:
 *   CONFIG_LVGL=y
 *   CONFIG_LVGL_DISPLAY=y
 *   CONFIG_GPIO=y
 *   CONFIG_RANDOM_NUMBER_GENERATOR=y
 *   CONFIG_2048_USE_BUTTONS_TO_MOVE=y  (optional)
 */

 #include "config.h"
 #include "game.h"
 #include "ui.h"
 #include "input.h"
 
 #include <zephyr/device.h>
 #include <zephyr/devicetree.h>
 #include <zephyr/drivers/display.h>
 #include <zephyr/kernel.h>
 #include <lvgl.h>
 #include <stdbool.h>
 
 /* Shared move flag, set by gesture_event_cb() or create_buttons() callbacks */
 int8_t direction_flag = 0;
 
 int main(void)
 {
     const struct device *dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
     if (!device_is_ready(dev)) {
         return 0;
     }
     display_blanking_off(dev);
 
     /* Let LVGL finish its initial timers */
     lv_timer_handler();
 
     /* Build the UI */
     lv_obj_t *scr = lv_scr_act();
     create_board(scr);
 #ifdef CONFIG_2048_USE_BUTTONS_TO_MOVE
     create_buttons(scr);
 #endif
     lv_obj_add_event_cb(scr, gesture_event_cb, LV_EVENT_GESTURE, NULL);
 
     /* Initialize game logic and first draw */
     init_game();
     update_ui();
 
     /* Main loop */
     while (1) {
         if (direction_flag) {
             bool moved = false;
             switch (direction_flag) {
             case 1: moved = move_left();  break;
             case 2: moved = move_up();    break;
             case 3: moved = move_right(); break;
             case 4: moved = move_down();  break;
             }
             if (moved) {
                 spawn_tile();
                 update_ui();
                 if (!can_move()) {
                     lv_obj_t *msg = lv_label_create(scr);
                     lv_label_set_text(msg, "Game Over");
                     lv_obj_center(msg);
                 }
             }
             direction_flag = 0;
         }
         lv_timer_handler();
         k_sleep(K_MSEC(50));
     }
 
     return 0;
 }
 