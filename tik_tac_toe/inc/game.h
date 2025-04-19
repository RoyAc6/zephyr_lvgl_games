#ifndef TTT_GAME_H
#define TTT_GAME_H

/**
 * @file game.h
 * @brief Tic‑Tac‑Toe game logic API.
 */

#include <stdbool.h>
#include <stdint.h>
#include "config.h"

/** Global board state:
 *  0 = empty, 1 = X, 2 = O
 */
extern int board_state[GRID_SIZE][GRID_SIZE];

/** Indicates whether the game is over (win or draw). */
extern bool game_over;

/**
 * @brief  Check if the last move at (r,c) has won.
 * @param  r   Row index of last move.
 * @param  c   Column index of last move.
 * @return     true if that move created a three‑in‑a‑row.
 */
bool ttt_check_win(int r, int c);

/**
 * @brief  Check if the board is full with no winner.
 * @return true if no empty cells remain.
 */
bool ttt_check_draw(void);

/**
 * @brief  Let the bot play one move (places an O randomly).
 * @param[out] r_out  Row index where the bot placed its O.
 * @param[out] c_out  Column index where the bot placed its O.
 * @return     true if a move was made, false if the board was full.
 */
bool ttt_bot_move(int *r_out, int *c_out);

/**
 * @brief  Reset board_state and game_over, returning to mode selection.
 */
void ttt_reset(void);

#endif /* TTT_GAME_H */
