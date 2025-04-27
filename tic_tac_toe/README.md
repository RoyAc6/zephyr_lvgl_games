# Tic‑Tac‑Toe on Zephyr + LVGL

A simple Tic‑Tac‑Toe game running on Zephyr RTOS with LVGL GUI.  
Supports 2‑player or vs‑bot mode, touch input, themable colors/fonts, and a styled end‑of‑game display.

---

## Features

- 3×3 grid rendered with LVGL  
- Two play modes:
  - **2 Players**: alternating X and O  
  - **Vs Bot**: you play X, the bot plays O (random moves)  
- Touch‑driven UI on `frdm_mcxn947` + `lcd_par_s035_8080`  
- Themed colors & fonts via `inc/config.h`  

---

## Hardware

- **Board**: NXP FRDM‑MCXN947 (CPU0)  
- **Display**: `lcd_par_s035_8080`  

