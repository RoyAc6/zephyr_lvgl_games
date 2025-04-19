/* src/game.c
 *
 * Implements the core 2048 game logic:
 * initialization, tile spawning, moves (left/right/up/down), and game‑over check.
 */

 #include "config.h"
 #include "game.h"
 #include <string.h>
 #include <zephyr/random/random.h>
 
 int grid[GRID_SIZE][GRID_SIZE];
 
 /**
  * @brief Reset the board and spawn two initial tiles.
  */
 void init_game(void)
 {
     /* Clear grid */
     memset(grid, 0, sizeof(grid));
 
     /* Place two initial tiles */
     for (int i = 0; i < 2; i++) {
         int empties = 0;
         int coords[GRID_SIZE * GRID_SIZE][2];
 
         /* Collect empty cell coordinates */
         for (int r = 0; r < GRID_SIZE; r++) {
             for (int c = 0; c < GRID_SIZE; c++) {
                 if (grid[r][c] == 0) {
                     coords[empties][0] = r;
                     coords[empties][1] = c;
                     empties++;
                 }
             }
         }
 
         if (empties == 0) {
             return;
         }
 
         /* Pick one at random */
         int idx = sys_rand32_get() % empties;
         /* 90% chance 2, 10% chance 4 */
         grid[coords[idx][0]][coords[idx][1]] =
             (sys_rand32_get() % 10 == 0) ? 4 : 2;
     }
 }
 
 /**
  * @brief Spawn one new tile (2 or 4) at a random empty position.
  */
 void spawn_tile(void)
 {
     int empties = 0;
     int coords[GRID_SIZE * GRID_SIZE][2];
 
     /* Collect empty cell coordinates */
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = 0; c < GRID_SIZE; c++) {
             if (grid[r][c] == 0) {
                 coords[empties][0] = r;
                 coords[empties][1] = c;
                 empties++;
             }
         }
     }
 
     if (empties == 0) {
         return;
     }
 
     /* Pick one at random */
     int idx = sys_rand32_get() % empties;
     /* 90% chance 2, 10% chance 4 */
     grid[coords[idx][0]][coords[idx][1]] =
         (sys_rand32_get() % 10 == 0) ? 4 : 2;
 }
 
 /**
  * @brief Shift and merge all rows to the left.
  *
  * @return true if any tile moved or merged, false otherwise.
  */
 bool move_left(void)
 {
     bool moved = false;
 
     for (int r = 0; r < GRID_SIZE; r++) {
         int tmp[GRID_SIZE] = {0};
         int cnt = 0;
 
         /* Compress non-zero tiles into tmp[] */
         for (int c = 0; c < GRID_SIZE; c++) {
             if (grid[r][c] != 0) {
                 tmp[cnt++] = grid[r][c];
             }
         }
 
         /* Merge adjacent equals */
         for (int i = 0; i < cnt - 1; i++) {
             if (tmp[i] && tmp[i] == tmp[i + 1]) {
                 tmp[i] *= 2;
                 tmp[i + 1] = 0;
                 moved = true;
             }
         }
 
         /* Rebuild the row and detect any movement */
         int final[GRID_SIZE] = {0};
         int j = 0;
         for (int i = 0; i < GRID_SIZE; i++) {
             if (tmp[i] != 0) {
                 final[j++] = tmp[i];
             }
         }
         for (int c = 0; c < GRID_SIZE; c++) {
             if (grid[r][c] != final[c]) {
                 moved = true;
                 grid[r][c] = final[c];
             }
         }
     }
 
     return moved;
 }
 
 /**
  * @brief Shift and merge all rows to the right.
  *
  * @return true if any tile moved or merged, false otherwise.
  */
 bool move_right(void)
 {
     /* Reverse each row, move left, reverse back */
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = 0; c < GRID_SIZE / 2; c++) {
             int t = grid[r][c];
             grid[r][c] = grid[r][GRID_SIZE - 1 - c];
             grid[r][GRID_SIZE - 1 - c] = t;
         }
     }
 
     bool moved = move_left();
 
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = 0; c < GRID_SIZE / 2; c++) {
             int t = grid[r][c];
             grid[r][c] = grid[r][GRID_SIZE - 1 - c];
             grid[r][GRID_SIZE - 1 - c] = t;
         }
     }
 
     return moved;
 }
 
 /**
  * @brief Shift and merge all columns upward.
  *
  * @return true if any tile moved or merged, false otherwise.
  */
 bool move_up(void)
 {
     /* Transpose, move left, transpose back */
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = r + 1; c < GRID_SIZE; c++) {
             int t = grid[r][c];
             grid[r][c] = grid[c][r];
             grid[c][r] = t;
         }
     }
 
     bool moved = move_left();
 
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = r + 1; c < GRID_SIZE; c++) {
             int t = grid[r][c];
             grid[r][c] = grid[c][r];
             grid[c][r] = t;
         }
     }
 
     return moved;
 }
 
 /**
  * @brief Shift and merge all columns downward.
  *
  * @return true if any tile moved or merged, false otherwise.
  */
 bool move_down(void)
 {
     /* Transpose, move right, transpose back */
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = r + 1; c < GRID_SIZE; c++) {
             int t = grid[r][c];
             grid[r][c] = grid[c][r];
             grid[c][r] = t;
         }
     }
 
     bool moved = move_right();
 
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = r + 1; c < GRID_SIZE; c++) {
             int t = grid[r][c];
             grid[r][c] = grid[c][r];
             grid[c][r] = t;
         }
     }
 
     return moved;
 }
 
 /**
  * @brief Check if any move is possible.
  *
  * Scans for any empty cell or any pair of equal adjacent tiles.
  *
  * @return true if at least one move remains possible.
  */
 bool can_move(void)
 {
     for (int r = 0; r < GRID_SIZE; r++) {
         for (int c = 0; c < GRID_SIZE; c++) {
             if (grid[r][c] == 0) {
                 return true;
             }
             if (c < GRID_SIZE - 1 && grid[r][c] == grid[r][c + 1]) {
                 return true;
             }
             if (r < GRID_SIZE - 1 && grid[r][c] == grid[r + 1][c]) {
                 return true;
             }
         }
     }
     return false;
 }
 