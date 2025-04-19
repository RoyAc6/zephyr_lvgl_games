/* src/ui.c
 *
 * Implements UI API for the Zephyr‑2048 project:
 * - get_bg_color(), get_text_color()
 * - create_board(), update_ui()
 */

 #include "config.h"
 #include "ui.h"
 #include "game.h"
 #include <lvgl.h>
 #include <stdio.h>
 
 static lv_obj_t *cells[GRID_SIZE][GRID_SIZE];
 
 /**
  * @brief Get the background color for a tile of value v.
  */
 lv_color_t get_bg_color(int v)
 {
     switch (v) {
     case 0:    return lv_color_hex(0xCDC1B4);
     case 2:    return lv_color_hex(0xEEE4DA);
     case 4:    return lv_color_hex(0xEDE0C8);
     case 8:    return lv_color_hex(0xF2B179);
     case 16:   return lv_color_hex(0xF59563);
     case 32:   return lv_color_hex(0xF67C5F);
     case 64:   return lv_color_hex(0xF65E3B);
     case 128:  return lv_color_hex(0xEDCF72);
     case 256:  return lv_color_hex(0xEDCC61);
     case 512:  return lv_color_hex(0xEDC850);
     case 1024: return lv_color_hex(0xEDC53F);
     case 2048: return lv_color_hex(0xEDC22E);
     default:   return lv_color_hex(0x3C3A32); /* >2048 */
     }
 }
 
 /**
  * @brief Get the text color for a tile of value v.
  */
 lv_color_t get_text_color(int v)
 {
     /* dark text for 2 and 4, light for larger tiles */
     return (v == 2 || v == 4)
            ? lv_color_hex(0x776E65)
            : lv_color_hex(0xF9F6F2);
 }
 
 /**
  * @brief Create and lay out the 4×4 grid of cells.
  */
 void create_board(lv_obj_t *parent)
 {
     lv_disp_t *disp = lv_disp_get_default();
     int scr_w = lv_disp_get_hor_res(disp);
     int scr_h = lv_disp_get_ver_res(disp);
     int board_w = (int)(scr_w * BOARD_SCALE);
     int board_h = (int)(scr_h * BOARD_SCALE);
     int cell_w = (board_w - (GRID_SIZE + 1) * CELL_MARGIN) / GRID_SIZE;
     int cell_h = (board_h - (GRID_SIZE + 1) * CELL_MARGIN) / GRID_SIZE;
 
     /* board background */
     lv_obj_t *board = lv_obj_create(parent);
     lv_obj_set_size(board, board_w, board_h);
     lv_obj_center(board);
     lv_obj_set_style_bg_color(board, lv_color_hex(0xBBADA0), 0);
     lv_obj_set_style_radius(board, 10, 0);
     lv_obj_set_style_pad_all(board, 0, 0);
 
     /* create each cell */
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = 0; c < GRID_SIZE; c++) {
             lv_obj_t *cell = lv_obj_create(board);
             lv_obj_set_size(cell, cell_w, cell_h);
             lv_obj_set_pos(cell,
                 CELL_MARGIN + c * (cell_w + CELL_MARGIN),
                 CELL_MARGIN + r * (cell_h + CELL_MARGIN));
             lv_obj_set_style_bg_color(cell, get_bg_color(0), 0);
             lv_obj_set_style_radius(cell, 5, 0);
             lv_obj_set_style_border_width(cell, 0, 0);
 
             /* label in center */
             lv_obj_t *lbl = lv_label_create(cell);
             lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_CENTER, 0);
             lv_obj_set_style_text_font(lbl, &lv_font_montserrat_22, 0);
             lv_obj_center(lbl);
 
             cells[r][c] = lbl;
         }
     }
 }
 
 /**
  * @brief Refresh all cell labels and backgrounds from the game state.
  */
 void update_ui(void)
 {
     char buf[8];
 
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = 0; c < GRID_SIZE; c++) {
             int v = grid[r][c];
             lv_obj_t *lbl  = cells[r][c];
             lv_obj_t *cell = lv_obj_get_parent(lbl);
 
             /* update background and text color */
             lv_obj_set_style_bg_color(cell, get_bg_color(v), 0);
             lv_obj_set_style_text_color(lbl, get_text_color(v), 0);
 
             /* update number or clear */
             if (v) {
                 snprintf(buf, sizeof(buf), "%d", v);
             } else {
                 buf[0] = '\0';
             }
             lv_label_set_text(lbl, buf);
         }
     }
 }
 