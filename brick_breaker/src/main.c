/*
 * Brick Breaker on Zephyr + LVGL
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <lvgl.h>

/* ─── Configuration ───────────────────────────────────────────────────────── */

#define BOARD_SCALE 1.0f /* fraction of screen used */
#define BRICK_ROWS 5
#define BRICK_COLS 8
#define BRICK_MARGIN 4       /* px between bricks */
#define PADDLE_WIDTH_SCALE 4 /* board_w / paddle_w */
#define PADDLE_HEIGHT 12     /* px */
#define HUD_MARGIN 24        /* px below paddle for HUD */
#define BALL_RADIUS 5        /* px */
#define INITIAL_BALL_VX 2.0f
#define INITIAL_BALL_VY -2.5f

/* ─── Theme ───────────────────────────────────────────────────────────────── */

typedef struct
{
    uint32_t board_bg_hex;
    uint32_t brick_hex;
    uint32_t paddle_hex;
    uint32_t ball_hex;
    uint32_t hud_text_hex;
} bb_theme_t;

static const bb_theme_t default_theme = {
    .board_bg_hex = 0x000000,
    .brick_hex = 0xFF5733,
    .paddle_hex = 0x33C1FF,
    .ball_hex = 0xFFFFFF,
    .hud_text_hex = 0xFFFFFF,
};

static const bb_theme_t *theme = &default_theme;

/* ─── Globals ─────────────────────────────────────────────────────────────── */

static lv_obj_t *board;
static lv_obj_t *bricks[BRICK_ROWS][BRICK_COLS];
static lv_obj_t *paddle;
static lv_obj_t *ball;
static lv_obj_t *score_label, *lives_label;

static int score = 0;
static int lives = 3;

static float ball_x, ball_y;
static float ball_vx, ball_vy;
static bool ball_launched = false;

static lv_timer_t *game_timer;

/* ─── Forward Declarations ────────────────────────────────────────────────── */

static void create_ui(lv_obj_t *parent);
static void reset_ball(void);
static void update_game(lv_timer_t *t);
static void paddle_touch_cb(lv_event_t *e);
static void restart_game_cb(lv_event_t *e);

/* ─── UI Creation ─────────────────────────────────────────────────────────── */

static void create_ui(lv_obj_t *parent)
{
    /* Screen dims */
    lv_disp_t *disp = lv_disp_get_default();
    int scr_w = lv_disp_get_hor_res(disp);
    int scr_h = lv_disp_get_ver_res(disp);

    /* Board area */
    lv_coord_t board_w = scr_w * BOARD_SCALE;
    lv_coord_t board_h = scr_h * BOARD_SCALE;

    board = lv_obj_create(parent);
    lv_obj_clear_flag(board, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_size(board, board_w, board_h);
    lv_obj_center(board);
    lv_obj_set_style_bg_color(board, lv_color_hex(theme->board_bg_hex), 0);
    lv_obj_set_style_pad_all(board, 0, 0);

    /* Bricks region */
    lv_coord_t bricks_h = board_h / 3;
    lv_coord_t brick_h = (bricks_h - (BRICK_ROWS + 1) * BRICK_MARGIN) / BRICK_ROWS;
    lv_coord_t brick_w = (board_w - (BRICK_COLS + 1) * BRICK_MARGIN) / BRICK_COLS;

    for (int r = 0; r < BRICK_ROWS; r++)
    {
        for (int c = 0; c < BRICK_COLS; c++)
        {
            lv_obj_t *b = lv_obj_create(board);
            lv_obj_set_size(b, brick_w, brick_h);
            lv_obj_set_style_bg_color(b, lv_color_hex(theme->brick_hex), 0);
            lv_obj_set_style_border_width(b, 0, 0);
            lv_coord_t x = BRICK_MARGIN + c * (brick_w + BRICK_MARGIN);
            lv_coord_t y = BRICK_MARGIN + r * (brick_h + BRICK_MARGIN);
            lv_obj_set_pos(b, x, y);
            bricks[r][c] = b;
        }
    }

    /* Paddle */
    lv_coord_t paddle_w = board_w / PADDLE_WIDTH_SCALE;
    paddle = lv_obj_create(board);
    lv_obj_set_size(paddle, paddle_w, PADDLE_HEIGHT);
    lv_obj_set_style_bg_color(paddle, lv_color_hex(theme->paddle_hex), 0);
    lv_obj_set_style_border_width(paddle, 0, 0);
    lv_obj_set_style_radius(paddle, PADDLE_HEIGHT / 2, 0);
    lv_coord_t py = board_h - PADDLE_HEIGHT - BRICK_MARGIN - HUD_MARGIN;
    lv_obj_set_pos(paddle, (board_w - paddle_w) / 2, py);

    /* Ball */
    ball = lv_obj_create(board);
    lv_obj_set_size(ball, BALL_RADIUS * 2, BALL_RADIUS * 2);
    lv_obj_set_style_bg_color(ball, lv_color_hex(theme->ball_hex), 0);
    lv_obj_set_style_radius(ball, BALL_RADIUS, 0);
    lv_obj_set_style_border_width(ball, 0, 0);
    reset_ball();

    /* HUD */
    score_label = lv_label_create(board);
    lv_label_set_text_fmt(score_label, "Score: %d", score);
    lv_obj_set_style_text_color(score_label, lv_color_hex(theme->hud_text_hex), 0);
    lv_obj_set_style_text_font(score_label, &lv_font_montserrat_14, 0);
    lv_obj_align(score_label, LV_ALIGN_BOTTOM_LEFT, 8, -8);

    lives_label = lv_label_create(board);
    lv_label_set_text_fmt(lives_label, "Lives: %d", lives);
    lv_obj_set_style_text_color(lives_label, lv_color_hex(theme->hud_text_hex), 0);
    lv_obj_set_style_text_font(lives_label, &lv_font_montserrat_14, 0);
    lv_obj_align(lives_label, LV_ALIGN_BOTTOM_RIGHT, -8, -8);

    /* Touch moves paddle (and launches ball on first touch) */
    lv_obj_add_event_cb(board, paddle_touch_cb, LV_EVENT_PRESSING, NULL);
}

/* ─── Game Logic ───────────────────────────────────────────────────────────── */

static void reset_ball(void)
{
    /* Position ball centered on paddle */
    lv_coord_t px = lv_obj_get_x(paddle);
    lv_coord_t pw = lv_obj_get_width(paddle);
    ball_x = px + pw / 2 - BALL_RADIUS;
    ball_y = lv_obj_get_y(paddle) - BALL_RADIUS * 2 - 1;
    lv_obj_set_pos(ball, (lv_coord_t)ball_x, (lv_coord_t)ball_y);
    ball_launched = false;
    /* zero velocity until launch */
    ball_vx = ball_vy = 0;
}

static void update_game(lv_timer_t *t)
{
    ARG_UNUSED(t);

    if (!ball_launched)
    {
        /* Follow paddle until first launch */
        lv_coord_t px = lv_obj_get_x(paddle);
        lv_coord_t pw = lv_obj_get_width(paddle);
        ball_x = px + pw / 2 - BALL_RADIUS;
        ball_y = lv_obj_get_y(paddle) - BALL_RADIUS * 2 - 1;
        lv_obj_set_pos(ball, (lv_coord_t)ball_x, (lv_coord_t)ball_y);
        return;
    }

    /* move ball */
    ball_x += ball_vx;
    ball_y += ball_vy;

    lv_coord_t bw = lv_obj_get_width(board);
    lv_coord_t bh = lv_obj_get_height(board);

    /* wall collisions */
    if (ball_x <= 0)
    {
        ball_x = 0;
        ball_vx = -ball_vx;
    }
    if (ball_x + BALL_RADIUS * 2 >= bw)
    {
        ball_x = bw - BALL_RADIUS * 2;
        ball_vx = -ball_vx;
    }
    if (ball_y <= 0)
    {
        ball_y = 0;
        ball_vy = -ball_vy;
    }

    /* brick collisions */
    for (int r = 0; r < BRICK_ROWS; r++)
    {
        for (int c = 0; c < BRICK_COLS; c++)
        {
            lv_obj_t *b = bricks[r][c];
            if (!lv_obj_has_flag(b, LV_OBJ_FLAG_HIDDEN))
            {
                lv_coord_t bx = lv_obj_get_x(b), by = lv_obj_get_y(b);
                lv_coord_t bwid = lv_obj_get_width(b), bhgt = lv_obj_get_height(b);
                if (ball_x + BALL_RADIUS * 2 > bx && ball_x < bx + bwid &&
                    ball_y + BALL_RADIUS * 2 > by && ball_y < by + bhgt)
                {
                    lv_obj_add_flag(b, LV_OBJ_FLAG_HIDDEN);
                    ball_vy = -ball_vy;
                    score += 10;
                    lv_label_set_text_fmt(score_label, "Score: %d", score);
                    goto after_brick;
                }
            }
        }
    }
after_brick:

    /* paddle collision */
    lv_coord_t px = lv_obj_get_x(paddle), py = lv_obj_get_y(paddle);
    lv_coord_t pw = lv_obj_get_width(paddle);
    if (ball_y + BALL_RADIUS * 2 >= py &&
        ball_x + BALL_RADIUS * 2 > px && ball_x < px + pw)
    {
        ball_y = py - BALL_RADIUS * 2 - 1;
        ball_vy = -ball_vy;
    }

    /* bottom: lose life */
    if (ball_y + BALL_RADIUS * 2 >= bh)
    {
        lives--;
        lv_label_set_text_fmt(lives_label, "Lives: %d", lives);
        /* If we detect "lives <= 0", show overlay and stop */
        if (lives <= 0)
        {
            /* 1) Stop game loop */
            lv_timer_del(game_timer);
            game_timer = NULL;

            lv_obj_t *scr = lv_scr_act();

            /* 2) Game Over container with white border */
            const lv_coord_t W = 200, H = 60;
            lv_obj_t *cont = lv_obj_create(scr);
            lv_obj_set_size(cont, W, H);
            lv_obj_center(cont);
            lv_obj_set_style_bg_color(cont, lv_color_hex(theme->board_bg_hex), 0);
            lv_obj_set_style_border_width(cont, 2, 0);
            lv_obj_set_style_border_color(cont, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_radius(cont, 8, 0);
            lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

            /* 3) "Game Over" label */
            lv_obj_t *lbl = lv_label_create(cont);
            lv_label_set_text(lbl, "Game Over");
            lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_text_font(lbl, &lv_font_montserrat_22, 0);
            lv_obj_center(lbl);

            /* 4) Restart button below */
            lv_obj_t *btn = lv_btn_create(scr);
            lv_obj_set_size(btn, 40, 40);
            lv_obj_align(btn, LV_ALIGN_CENTER, 0, H / 2 + 20);
            lv_obj_add_event_cb(btn, restart_game_cb, LV_EVENT_CLICKED, NULL);

            /* make it transparent except for icon */
            lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, 0);
            lv_obj_set_style_border_opa(btn, LV_OPA_TRANSP, 0);
            lv_obj_set_style_shadow_opa(btn, LV_OPA_TRANSP, 0);

            lv_obj_t *icon = lv_label_create(btn);
            lv_label_set_text(icon, LV_SYMBOL_REFRESH);
            lv_obj_set_style_text_color(icon, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_text_font(icon, &lv_font_montserrat_22, 0);
            lv_obj_center(icon);

            return;
        }
        reset_ball();
    }

    lv_obj_set_pos(ball, (lv_coord_t)ball_x, (lv_coord_t)ball_y);
}

/* ─── Touch / Launch ───────────────────────────────────────────────────────── */

static void paddle_touch_cb(lv_event_t *e)
{
    lv_point_t p;
    lv_indev_t *indev = lv_indev_get_act();
    lv_indev_get_point(indev, &p);

    /* compute local x */
    lv_coord_t bx = lv_obj_get_x(board);
    lv_coord_t lx = p.x - bx;
    lv_coord_t pw = lv_obj_get_width(paddle);

    /* on first touch, launch ball */
    if (!ball_launched)
    {
        ball_launched = true;
        ball_vx = INITIAL_BALL_VX;
        ball_vy = INITIAL_BALL_VY;
    }

    /* move paddle */
    lv_coord_t new_x = lx - pw / 2;
    if (new_x < 0)
        new_x = 0;
    if (new_x + pw > lv_obj_get_width(board))
        new_x = lv_obj_get_width(board) - pw;
    lv_obj_set_x(paddle, new_x);
}

static void restart_game_cb(lv_event_t *e)
{
    ARG_UNUSED(e);

    /* 1) Stop any existing timer */
    if (game_timer)
    {
        lv_timer_del(game_timer);
        game_timer = NULL;
    }

    /* 2) Reset game state */
    score = 0;
    lives = 3;
    ball_launched = false;

    /* 3) Clear entire screen and rebuild everything */
    lv_obj_clean(lv_scr_act());
    create_ui(lv_scr_act());

    /* 4) Start a single new timer */
    game_timer = lv_timer_create(update_game, 20, NULL);
}

/* ─── Main ───────────────────────────────────────────────────────────────── */

int main(void)
{
    const struct device *disp = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (!device_is_ready(disp))
    {
        printk("Display not ready\n");
        return -1;
    }
    display_blanking_off(disp);

    lv_timer_handler();

    lv_obj_t *scr = lv_scr_act();
    create_ui(scr);

    game_timer = lv_timer_create(update_game, 20, NULL);

    while (1)
    {
        lv_timer_handler();
        k_sleep(K_MSEC(10));
    }
    return 0;
}
