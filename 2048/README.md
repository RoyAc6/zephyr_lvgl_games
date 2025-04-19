# Zephyr2048

A Zephyr RTOS port of the classic **2048** game, powered by [LVGL](https://lvgl.io/) on the **FRDM_MCXN947** development board with the **lcd_par_s035_8080** shield.

![2048 on Zephyr](assets/screenshot.png)

## Features

- **Smooth 4×4 grid** rendered with LVGL  
- **Classic 2048 color palette** and tile animations  
- **Swipe‑to‑move** gesture support  
- **Optional on‑screen arrow buttons** (enable via `CONFIG_2048_USE_BUTTONS_TO_MOVE`)  
- **Game‑over detection** and “Game Over” overlay  