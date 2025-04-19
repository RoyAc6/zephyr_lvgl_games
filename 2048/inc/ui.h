/* include/ui.h
 *
 * UI API for the Zephyr‑2048 project.
 * Handles creation of the LVGL board, tile coloring, and UI updates.
 */

 #ifndef UI_H
 #define UI_H
 
 #include "config.h"
 #include <lvgl.h>
 
 /**
  * @brief Get the background color for a tile of value @p v.
  *
  * Returns the standard 2048 hex color for values 0,2,4,…,2048,
  * or a default dark color for values >2048.
  *
  * @param v The tile value (0 = empty, otherwise power of two).
  * @return An lv_color_t representing the tile background.
  */
 lv_color_t get_bg_color(int v);
 
 /**
  * @brief Get the text color for a tile of value @p v.
  *
  * Returns a dark text color for small tiles (2,4) and light
  * for larger ones, matching the classic 2048 palette.
  *
  * @param v The tile value.
  * @return An lv_color_t for the label text.
  */
 lv_color_t get_text_color(int v);
 
 /**
  * @brief Create and lay out the 4×4 grid of cells.
  *
  * Allocates a parent “board” object (centered and scaled by BOARD_SCALE)
  * then creates GRID_SIZE × GRID_SIZE child cells with margins of CELL_MARGIN.
  * Each cell holds an empty label initially.
  *
  * @param parent The LVGL object (e.g. lv_scr_act()) to attach the board to.
  */
 void create_board(lv_obj_t *parent);
 
 /**
  * @brief Refresh all cell labels and backgrounds from the game state.
  *
  * Reads the global `grid[GRID_SIZE][GRID_SIZE]` array and for each cell:
  *  - Sets its background color via get_bg_color()
  *  - Sets its label text to the tile value (or empty)
  *  - Sets its label text color via get_text_color()
  */
 void update_ui(void);
 
 #endif /* UI_H */
 