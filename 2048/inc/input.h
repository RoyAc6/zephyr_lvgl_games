/* include/input.h
 *
 * Input handling API for the Zephyr‑2048 project.
 * Supports swipe gestures and optional on‑screen buttons.
 */

 #ifndef INPUT_H
 #define INPUT_H
 
 #include "config.h"
 #include <lvgl.h>
 
 /**
  * @brief LVGL gesture event callback to handle swipe moves.
  *
  * Attach this to the active screen with:
  *     lv_obj_add_event_cb(scr, gesture_event_cb, LV_EVENT_GESTURE, NULL);
  *
  * It reads the last gesture direction from the active input device
  * and sets the internal direction_flag (1=left,2=up,3=right,4=down).
  *
  * @param e Pointer to the LVGL event containing gesture information.
  */
 void gesture_event_cb(lv_event_t *e);
 
 #ifdef CONFIG_2048_USE_BUTTONS_TO_MOVE
 
 /**
  * @brief Create transparent edge buttons for tap-based movement.
  *
  * Each button spans one side of the board (overlapping it by BTN_OVERLAP)
  * and uses an LV_SYMBOL_* arrow. Touching a button sets the move direction.
  *
  * @param parent LVGL parent object (typically lv_scr_act()) to which buttons are added.
  */
 void create_buttons(lv_obj_t *parent);
 
 #endif /* CONFIG_2048_USE_BUTTONS_TO_MOVE */
 
 #endif /* INPUT_H */
 