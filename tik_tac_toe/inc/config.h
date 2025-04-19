/* include/config.h */
#ifndef TTT_CONFIG_H
#define TTT_CONFIG_H

/**
 * @file config.h
 * @brief Common configuration macros for Tic‑Tac‑Toe (grid size, theming).
 */

/** @def GRID_SIZE
 *  @brief Number of rows/columns in the board (3×3).
 */
#define GRID_SIZE    3

/** @def BOARD_SCALE
 *  @brief Fraction of screen width/height occupied by the board container.
 */
#define BOARD_SCALE  0.8f

/** @def CELL_MARGIN
 *  @brief Pixels of spacing between cells in the board.
 */
#define CELL_MARGIN  5

/** @def X_SYMBOL_COLOR_HEX
 *  @brief 24‑bit RGB color for X symbols (hex).
 */
#define X_SYMBOL_COLOR_HEX        0xFF3B30

/** @def O_SYMBOL_COLOR_HEX
 *  @brief 24‑bit RGB color for O symbols (hex).
 */
#define O_SYMBOL_COLOR_HEX        0x007AFF

/** @def BOARD_BG_COLOR_HEX
 *  @brief 24‑bit RGB color for the board background (hex).
 */
#define BOARD_BG_COLOR_HEX        0x222222

/** @def CELL_BG_COLOR_HEX
 *  @brief 24‑bit RGB color for empty cell background (hex).
 */
#define CELL_BG_COLOR_HEX         0xF5E6FF

/** @def CELL_BORDER_COLOR_HEX
 *  @brief 24‑bit RGB color for cell borders (hex).
 */
#define CELL_BORDER_COLOR_HEX     0xFFFFFF

/** @def X_SYMBOL_FONT
 *  @brief LVGL font pointer to use for X symbols.
 */
#define X_SYMBOL_FONT             &lv_font_montserrat_32

/** @def O_SYMBOL_FONT
 *  @brief LVGL font pointer to use for O symbols.
 */
#define O_SYMBOL_FONT             &lv_font_montserrat_32

/** @def RESULT_FONT
 *  @brief LVGL font pointer to use for result messages.
 */
#define RESULT_FONT               &lv_font_montserrat_32

#endif /* TTT_CONFIG_H */
