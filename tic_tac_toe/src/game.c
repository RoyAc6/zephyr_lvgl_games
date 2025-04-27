#include "game.h"
#include <string.h>
#include <zephyr/random/random.h>

int board_state[GRID_SIZE][GRID_SIZE];
bool game_over = false;

bool ttt_check_win(int r, int c)
{
    int p = board_state[r][c];
    bool win;

    /* Row */
    win = true;
    for (int i = 0; i < GRID_SIZE; i++) {
        if (board_state[r][i] != p) {
            win = false;
            break;
        }
    }
    if (win) return true;

    /* Column */
    win = true;
    for (int i = 0; i < GRID_SIZE; i++) {
        if (board_state[i][c] != p) {
            win = false;
            break;
        }
    }
    if (win) return true;

    /* Main diagonal */
    if (r == c) {
        win = true;
        for (int i = 0; i < GRID_SIZE; i++) {
            if (board_state[i][i] != p) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    /* Anti‑diagonal */
    if (r + c == GRID_SIZE - 1) {
        win = true;
        for (int i = 0; i < GRID_SIZE; i++) {
            if (board_state[i][GRID_SIZE - 1 - i] != p) {
                win = false;
                break;
            }
        }
        if (win) return true;
    }

    return false;
}

bool ttt_check_draw(void)
{
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (board_state[r][c] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool ttt_bot_move(int *r_out, int *c_out)
{
    int empties = 0;
    int coords[GRID_SIZE * GRID_SIZE][2];

    /* Gather empty cells */
    for (int r = 0; r < GRID_SIZE; r++) {
        for (int c = 0; c < GRID_SIZE; c++) {
            if (board_state[r][c] == 0) {
                coords[empties][0] = r;
                coords[empties][1] = c;
                empties++;
            }
        }
    }

    if (empties == 0) {
        return false;
    }

    /* Pick a random empty cell */
    int idx = sys_rand32_get() % empties;
    int r = coords[idx][0];
    int c = coords[idx][1];

    /* Place O */
    board_state[r][c] = 2;
    *r_out = r;
    *c_out = c;
    return true;
}

void ttt_reset(void)
{
    memset(board_state, 0, sizeof(board_state));
    game_over = false;
}
