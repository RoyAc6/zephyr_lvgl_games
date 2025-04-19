/* src/input.c
 *
 * Implements input handling for the Zephyr‑2048 project.
 * Supports swipe gestures and optional on‑screen buttons.
 */

 #include "config.h"
 #include "input.h"
 #include <lvgl.h>
 #include <stdint.h>
 
 /* direction_flag is defined in main.c */
 extern int8_t direction_flag;
 
 void gesture_event_cb(lv_event_t *e)
 {
     /* Get the active input device and query its last gesture */
     lv_indev_t *indev = lv_indev_get_act();
     lv_dir_t dir = lv_indev_get_gesture_dir(indev);
 
     switch (dir) {
     case LV_DIR_LEFT:   direction_flag = 1; break;
     case LV_DIR_TOP:    direction_flag = 2; break;
     case LV_DIR_RIGHT:  direction_flag = 3; break;
     case LV_DIR_BOTTOM: direction_flag = 4; break;
     default: break;
     }
 }
 
 #ifdef CONFIG_2048_USE_BUTTONS_TO_MOVE
 
 /* LVGL animation callback to scale button */
 static void zoom_anim_cb(void *var, int32_t v)
 {
     lv_obj_set_style_transform_zoom((lv_obj_t *)var, v, 0);
 }
 
 /* Animate button press */
 static void animate_button(lv_obj_t *btn)
 {
     lv_anim_t a;
     lv_anim_init(&a);
     lv_anim_set_var(&a, btn);
     lv_anim_set_exec_cb(&a, zoom_anim_cb);
     lv_anim_set_values(&a, 256, 218); /* 100% to ~85% */
     lv_anim_set_time(&a, 100);
     lv_anim_set_playback_time(&a, 100);
     lv_anim_set_playback_delay(&a, 0);
     lv_anim_start(&a);
 }
 
 /* Direction button event: animate and set flag */
 static void btn_event_cb(lv_event_t *e)
 {
     lv_obj_t *btn = lv_event_get_target(e);
     animate_button(btn);
     direction_flag = (int)(intptr_t)lv_event_get_user_data(e);
 }
 
 /* Create transparent edge buttons for tap-based movement */
 void create_buttons(lv_obj_t *parent)
 {
     /* Compute screen and board geometry */
     lv_disp_t *disp = lv_disp_get_default();
     int scr_w   = lv_disp_get_hor_res(disp);
     int scr_h   = lv_disp_get_ver_res(disp);
     int board_w = (int)(scr_w * BOARD_SCALE);
     int board_h = (int)(scr_h * BOARD_SCALE);
 
     /* Margins around the board */
     int margin_h = (scr_w - board_w) / 2;
     int margin_v = (scr_h - board_h) / 2;
 
     /* All buttons get the same “thickness”, overlapping the board */
     int thickness = (margin_h > margin_v ? margin_h : margin_v) + BTN_OVERLAP;
 
     lv_obj_t *btn;
     lv_obj_t *label;
 
     /* Helper to fully hide a button’s visuals */
     #define MAKE_BTN_INVISIBLE(b)                            \
         do {                                                \
             lv_obj_set_style_bg_opa(b,    LV_OPA_TRANSP, 0); \
             lv_obj_set_style_border_opa(b,LV_OPA_TRANSP, 0); \
             lv_obj_set_style_outline_opa(b,LV_OPA_TRANSP, 0); \
             lv_obj_set_style_shadow_opa(b, LV_OPA_TRANSP, 0); \
             lv_obj_set_style_border_width(b,  0, 0);        \
             lv_obj_set_style_outline_width(b, 0, 0);        \
             lv_obj_set_style_shadow_width(b,  0, 0);        \
             lv_obj_set_style_radius(b,        0, 0);        \
             lv_obj_set_style_pad_all(b,       0, 0);        \
         } while (0)
 
     /* ─── Up ─────────────────────────────────────────────────── */
     btn = lv_btn_create(parent);
     lv_obj_set_size(btn, board_w, thickness);
     lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 0);
     MAKE_BTN_INVISIBLE(btn);
     lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)2);
     label = lv_label_create(btn);
     lv_label_set_text(label, LV_SYMBOL_UP);
     lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
     lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, -SYMBOL_PAD - SYMBOL_ADJUSTMENT);
 
     /* ─── Down ───────────────────────────────────────────────── */
     btn = lv_btn_create(parent);
     lv_obj_set_size(btn, board_w, thickness);
     lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, 0);
     MAKE_BTN_INVISIBLE(btn);
     lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)4);
     label = lv_label_create(btn);
     lv_label_set_text(label, LV_SYMBOL_DOWN);
     lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
     lv_obj_align(label, LV_ALIGN_TOP_MID, 0, SYMBOL_PAD + SYMBOL_ADJUSTMENT);
 
     /* ─── Left ───────────────────────────────────────────────── */
     btn = lv_btn_create(parent);
     lv_obj_set_size(btn, thickness, board_h);
     lv_obj_align(btn, LV_ALIGN_LEFT_MID, 0, 0);
     MAKE_BTN_INVISIBLE(btn);
     lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)1);
     label = lv_label_create(btn);
     lv_label_set_text(label, LV_SYMBOL_LEFT);
     lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
     lv_obj_align(label, LV_ALIGN_RIGHT_MID, -SYMBOL_PAD, 0);
 
     /* ─── Right ─────────────────────────────────────────────── */
     btn = lv_btn_create(parent);
     lv_obj_set_size(btn, thickness, board_h);
     lv_obj_align(btn, LV_ALIGN_RIGHT_MID, 0, 0);
     MAKE_BTN_INVISIBLE(btn);
     lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, (void*)(intptr_t)3);
     label = lv_label_create(btn);
     lv_label_set_text(label, LV_SYMBOL_RIGHT);
     lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
     lv_obj_align(label, LV_ALIGN_LEFT_MID, SYMBOL_PAD, 0);
 
     #undef MAKE_BTN_INVISIBLE
 }
 
 #endif /* CONFIG_2048_USE_BUTTONS_TO_MOVE */
 