#include "input.h"
#include "game.h"
#include "ui.h"
#include "config.h"
#include <lvgl.h>

/* Shared objects from ui.c and main.c */
extern lv_obj_t *scr;
extern lv_obj_t *cells[GRID_SIZE][GRID_SIZE];
extern bool game_over;

/* Current game mode: -1=menu, 0=2P, 1=vs bot */
int game_mode = -1;

void reset_cb(lv_event_t *e)
{
    ARG_UNUSED(e);
    ttt_reset();
    game_mode = -1;
    lv_obj_clean(scr);
    create_board_ui(scr);
}

void mode_select_cb(lv_event_t *e)
{
    game_mode = (intptr_t)lv_event_get_user_data(e);
    lv_obj_clean(scr);
    create_board_ui(scr);
}

void cell_event_cb(lv_event_t *e)
{
    if (game_over) {
        return;
    }

    intptr_t idx = (intptr_t)lv_event_get_user_data(e);
    int r = idx / GRID_SIZE;
    int c = idx % GRID_SIZE;
    if (board_state[r][c] != 0) {
        return;
    }

    if (game_mode == 0) {
        /* Two‑player mode */
        static bool x_turn = true;
        board_state[r][c] = x_turn ? 1 : 2;
        lv_label_set_text(cells[r][c], x_turn ? "X" : "O");
        uint32_t col = x_turn ? X_SYMBOL_COLOR_HEX : O_SYMBOL_COLOR_HEX;
        const lv_font_t *f = x_turn ? X_SYMBOL_FONT : O_SYMBOL_FONT;
        lv_obj_set_style_text_color(cells[r][c], lv_color_hex(col), 0);
        lv_obj_set_style_text_font(cells[r][c], f, 0);

        if (ttt_check_win(r, c)) {
            game_over = true;
            show_result_ui(x_turn ? "X wins!" : "O wins!", col, col);
            return;
        }
        if (ttt_check_draw()) {
            game_over = true;
            show_result_ui("Draw!", BOARD_BG_COLOR_HEX, BOARD_BG_COLOR_HEX);
            return;
        }
        x_turn = !x_turn;

    } else {
        /* Vs Bot mode */
        board_state[r][c] = 1;
        lv_label_set_text(cells[r][c], "X");
        lv_obj_set_style_text_color(cells[r][c], lv_color_hex(X_SYMBOL_COLOR_HEX), 0);
        lv_obj_set_style_text_font(cells[r][c], X_SYMBOL_FONT, 0);

        if (ttt_check_win(r, c)) {
            game_over = true;
            show_result_ui("You win!", X_SYMBOL_COLOR_HEX, X_SYMBOL_COLOR_HEX);
            return;
        }
        if (ttt_check_draw()) {
            game_over = true;
            show_result_ui("Draw!", CELL_BORDER_COLOR_HEX, CELL_BORDER_COLOR_HEX);
            return;
        }

        /* Bot plays */
        int br, bc;
        if (ttt_bot_move(&br, &bc)) {
            /* Draw O on UI */
            lv_label_set_text(cells[br][bc], "O");
            lv_obj_set_style_text_color(cells[br][bc], lv_color_hex(O_SYMBOL_COLOR_HEX), 0);
            lv_obj_set_style_text_font(cells[br][bc], O_SYMBOL_FONT, 0);

            if (ttt_check_win(br, bc)) {
                game_over = true;
                show_result_ui("Bot wins!", O_SYMBOL_COLOR_HEX, O_SYMBOL_COLOR_HEX);
                return;
            }
        }

        if (ttt_check_draw()) {
            game_over = true;
            show_result_ui("Draw!", CELL_BORDER_COLOR_HEX, CELL_BORDER_COLOR_HEX);
            return;
        }
    }
}
