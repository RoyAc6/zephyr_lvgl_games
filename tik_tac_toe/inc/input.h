/* include/input.h */
#ifndef TTT_INPUT_H
#define TTT_INPUT_H

/**
 * @file input.h
 * @brief Tic‑Tac‑Toe input event callbacks.
 */

#include <lvgl.h>

/** Current game mode:
 *  -1 = show menu, 0 = two‑player, 1 = versus bot.
 */
extern int game_mode;

/** @brief  LVGL event callback for when a cell is tapped.
 *  @param  e  LVGL event containing user_data = r*GRID_SIZE + c. */
void cell_event_cb(lv_event_t *e);

/** @brief  LVGL event callback for selecting mode (2P vs bot).
 *  @param  e  LVGL event containing user_data = 0 or 1. */
void mode_select_cb(lv_event_t *e);

/** @brief  LVGL event callback to reset the game (back to menu). */
void reset_cb(lv_event_t *e);

#endif /* TTT_INPUT_H */
