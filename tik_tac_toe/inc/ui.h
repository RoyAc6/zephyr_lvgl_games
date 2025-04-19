/* include/ui.h */
#ifndef TTT_UI_H
#define TTT_UI_H

/**
 * @file ui.h
 * @brief Tic‑Tac‑Toe UI rendering API using LVGL.
 */

#include <lvgl.h>

/** @brief  Create either the mode‑select menu or the game board,
 *          depending on game_mode. */
void create_board_ui(lv_obj_t *parent);

/** @brief  Display end‑of‑game result in a styled capsule and show reset icon.
 *  @param  text       Zero‑terminated result string, e.g. "X wins!".
 *  @param  border_hex 24‑bit RGB border color (hex).
 *  @param  text_hex   24‑bit RGB text color (hex).
 */
void show_result_ui(const char *text,
                    uint32_t border_hex,
                    uint32_t text_hex);

#endif /* TTT_UI_H */
