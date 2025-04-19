/* include/config.h
 *
 * Common configuration defines for the Zephyr‑2048 project
 * 
 */

 #ifndef CONFIG_H
 #define CONFIG_H
 
 /* Grid dimensions */
 #define GRID_SIZE       4
 #define CELL_MARGIN     5   /* px between cells */
 
 /*
  * Choose how much of the screen the board occupies.
  * Set CONFIG_2048_USE_BUTTONS_TO_MOVE=y in prj.conf to enable on‑screen buttons.
  */
 #ifdef CONFIG_2048_USE_BUTTONS_TO_MOVE
 #  define BOARD_SCALE    0.80f  /* 80% of screen when using on‑screen buttons */
 #else
 #  define BOARD_SCALE    0.94f  /* 94% of screen when using swipe only */
 #endif
 
 /*
  * Button overlap and symbol positioning
  * (only used if CONFIG_2048_USE_BUTTONS_TO_MOVE is enabled)
  */
 #define BTN_OVERLAP         10    /* px buttons extend over the grid edge */
 #define SYMBOL_PAD          14    /* px from inner board edge to arrow */
 #define SYMBOL_ADJUSTMENT   14    /* additional px adj. for top/bottom arrows */
 
 #endif /* CONFIG_H */
 