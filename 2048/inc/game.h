#ifndef GAME_H
#define GAME_H

/**
 * @file game.h
 * @brief Game logic API for the Zephyr‑2048 project.
 *
 * Implements the core 2048 mechanics: initialization, moves, spawning, and game‑over check.
 */

#include "config.h"
#include <stdbool.h>

/**
 * @brief The current board state.
 *
 * A 4×4 array where 0 represents an empty cell, and any other value
 * represents a tile of that value.
 */
extern int grid[GRID_SIZE][GRID_SIZE];

/**
 * @brief Reset the board and spawn two initial tiles.
 *
 * Clears the grid to all zeros and then randomly places two tiles
 * (2 or, with 10% chance, 4) on empty cells.
 */
void init_game(void);

/**
 * @brief Add one new tile (2 or 4) at a random empty position.
 *
 * Chooses a random empty cell and places a 2 (90% chance) or 4 (10% chance).
 */
void spawn_tile(void);

/**
 * @brief Shift and merge all rows to the left.
 *
 * Compresses non-zero tiles leftward, merges equal adjacent tiles into one
 * tile of double value, and compresses again.
 *
 * @return true if any tile moved or merged, false otherwise.
 */
bool move_left(void);

/**
 * @brief Shift and merge all rows to the right.
 *
 * Mirrors the move_left algorithm: reverses each row, applies move_left,
 * then reverses back.
 *
 * @return true if any tile moved or merged, false otherwise.
 */
bool move_right(void);

/**
 * @brief Shift and merge all columns upward.
 *
 * Transposes the grid, applies move_left, then transposes back.
 *
 * @return true if any tile moved or merged, false otherwise.
 */
bool move_up(void);

/**
 * @brief Shift and merge all columns downward.
 *
 * Transposes the grid, applies move_right, then transposes back.
 *
 * @return true if any tile moved or merged, false otherwise.
 */
bool move_down(void);

/**
 * @brief Check if any move is possible.
 *
 * Scans for any empty cell (value 0) or any pair of equal adjacent tiles
 * horizontally or vertically.
 *
 * @return true if at least one move remains possible, false if the game is over.
 */
bool can_move(void);

#endif /* GAME_H */
