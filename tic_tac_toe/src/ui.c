/* src/ui.c
 * Implements Tic‑Tac‑Toe UI (mode menu, board, result) using LVGL.
 */

 #include "ui.h"
 #include "config.h"
 #include "input.h"
 #include "game.h"
 #include <lvgl.h>
 
 lv_obj_t *cells[GRID_SIZE][GRID_SIZE];  /* cell label objects */
 extern lv_obj_t *scr;


 /** Theme data structure */
 typedef struct {
     uint32_t         x_color_hex;
     uint32_t         o_color_hex;
     const lv_font_t *x_font;
     const lv_font_t *o_font;
     uint32_t         board_bg_color_hex;
     uint32_t         cell_bg_color_hex;
     uint32_t         cell_border_color_hex;
 } theme_t;
 
 /** Default theme instance */
 static const theme_t default_theme = {
     .x_color_hex           = X_SYMBOL_COLOR_HEX,
     .o_color_hex           = O_SYMBOL_COLOR_HEX,
     .x_font                = X_SYMBOL_FONT,
     .o_font                = O_SYMBOL_FONT,
     .board_bg_color_hex    = BOARD_BG_COLOR_HEX,
     .cell_bg_color_hex     = CELL_BG_COLOR_HEX,
     .cell_border_color_hex = CELL_BORDER_COLOR_HEX,
 };
 
 /** Active theme pointer (can be swapped) */
 static const theme_t *theme = &default_theme;
 
 void create_board_ui(lv_obj_t *parent)
 {
     /* Mode selection menu */
     if (game_mode < 0) {
         const int btn_w = 120, btn_h = 50, spacing = 20;
         lv_obj_t *btn, *lbl;
 
         /* 2 Players button */
         btn = lv_btn_create(parent);
         lv_obj_set_size(btn, btn_w, btn_h);
         lv_obj_align(btn, LV_ALIGN_CENTER, -(btn_w/2 + spacing/2), 0);
         lv_obj_add_event_cb(btn, mode_select_cb, LV_EVENT_CLICKED, (void*)0);
         lbl = lv_label_create(btn);
         lv_label_set_text(lbl, "2 Players");
         lv_obj_center(lbl);
 
         /* Vs Bot button */
         btn = lv_btn_create(parent);
         lv_obj_set_size(btn, btn_w, btn_h);
         lv_obj_align(btn, LV_ALIGN_CENTER,  (btn_w/2 + spacing/2), 0);
         lv_obj_add_event_cb(btn, mode_select_cb, LV_EVENT_CLICKED, (void*)1);
         lbl = lv_label_create(btn);
         lv_label_set_text(lbl, "Vs Bot");
         lv_obj_center(lbl);
 
         return;
     }
 
     /* Game board */
     lv_disp_t *disp = lv_disp_get_default();
     int sw = lv_disp_get_hor_res(disp);
     int sh = lv_disp_get_ver_res(disp);
     int bw = (int)(sw * BOARD_SCALE);
     int bh = (int)(sh * BOARD_SCALE);
     int cw = (bw - (GRID_SIZE+1)*CELL_MARGIN) / GRID_SIZE;
     int ch = (bh - (GRID_SIZE+1)*CELL_MARGIN) / GRID_SIZE;
 
     /* Clear any old game state */
     ttt_reset();
 
     lv_obj_t *board = lv_obj_create(parent);
     lv_obj_set_size(board, bw, bh);
     lv_obj_center(board);
     lv_obj_set_style_bg_color(board, lv_color_hex(theme->board_bg_color_hex), 0);
     lv_obj_set_style_pad_all(board, 0, 0);
 
     /* Create 3×3 cells */
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = 0; c < GRID_SIZE; c++) {
             lv_obj_t *cell = lv_btn_create(board);
             lv_obj_set_size(cell, cw, ch);
             lv_obj_set_pos(cell,
                 CELL_MARGIN + c*(cw + CELL_MARGIN),
                 CELL_MARGIN + r*(ch + CELL_MARGIN));
             lv_obj_set_style_radius(cell,       0, 0);
             lv_obj_set_style_border_width(cell, 1, 0);
             lv_obj_set_style_border_color(cell, lv_color_hex(theme->cell_border_color_hex), 0);
             lv_obj_set_style_bg_color(cell,     lv_color_hex(theme->cell_bg_color_hex),    0);
 
             /* Empty label to hold X/O */
             lv_obj_t *lbl = lv_label_create(cell);
             lv_label_set_text(lbl, " ");
             lv_obj_center(lbl);
             cells[r][c] = lbl;
 
             /* Hook tap event */
             lv_obj_add_event_cb(cell, cell_event_cb, LV_EVENT_CLICKED,
                                 (void*)(intptr_t)(r * GRID_SIZE + c));
         }
     }
 }
 
 void show_result_ui(const char *text,
                     uint32_t border_hex,
                     uint32_t text_hex)
 {
     lv_coord_t scr_w = lv_disp_get_hor_res(lv_disp_get_default());
     lv_coord_t cont_w = scr_w * 3 / 4;
     lv_coord_t cont_h =  60;  /* capsule height */
 
     /* Capsule container */
     lv_obj_t *cont = lv_obj_create(scr);
     lv_obj_set_size(cont, cont_w, cont_h);
     lv_obj_center(cont);
     lv_obj_set_style_bg_color(cont, lv_color_hex(theme->cell_bg_color_hex), 0);
     lv_obj_set_style_border_width(cont, 2, 0);
     lv_obj_set_style_border_color(cont, lv_color_hex(border_hex), 0);
     lv_obj_set_style_radius(cont, cont_h/2, 0);
     lv_obj_set_style_pad_all(cont, 10, 0);
     lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
 
     /* Result text */
     lv_obj_t *lbl = lv_label_create(cont);
     lv_label_set_text(lbl, text);
     lv_label_set_long_mode(lbl, LV_LABEL_LONG_WRAP);
     lv_obj_set_width(lbl, cont_w - 20);
     lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
     lv_obj_set_style_text_color(lbl, lv_color_hex(text_hex), 0);
     lv_obj_set_style_text_font(lbl, RESULT_FONT, 0);
     lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);
 
     /* Restart icon button */
     lv_obj_t *btn = lv_btn_create(scr);
     lv_obj_set_size(btn, 40, 40);
     lv_obj_align(btn, LV_ALIGN_CENTER, 0, cont_h/2 + 30);
     lv_obj_add_event_cb(btn, reset_cb, LV_EVENT_CLICKED, NULL);
     lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
     lv_obj_set_style_border_opa(btn, LV_OPA_TRANSP, 0);
     lv_obj_set_style_shadow_opa(btn, LV_OPA_TRANSP, 0);
 
     lv_obj_t *icon = lv_label_create(btn);
     lv_label_set_text(icon, LV_SYMBOL_REFRESH);
     lv_obj_set_style_text_color(icon, lv_color_hex(theme->board_bg_color_hex), 0);
     lv_obj_set_style_text_font(icon, RESULT_FONT, 0);
     lv_obj_center(icon);
 }
 