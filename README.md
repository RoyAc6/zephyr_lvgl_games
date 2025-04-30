# Zephyr + LVGL Games Suite
A collection of simple games built on Zephyr RTOS with the LVGL graphics library.

## Hardware

- **Board:** [FRDM_MCXN947 (NXP MCU)](https://www.nxp.com/design/design-center/development-boards-and-designs/FRDM-MCXN947)
- **Shield:** [lcd_par_s035_8080](https://www.nxp.com/design/design-center/development-boards-and-designs/LCD-PAR-S035)

## Getting Started

```bash
west init -l zephyr_lvgl_games
west update
west build --board frdm_mcxn947/mcxn947/cpu0 <game_folder>
```

## Known Issues & Workarounds

### Touch controller timing tweak

If you see missed touches, edit  
`zephyr/drivers/input/input_gt911.c` and locate the comment:

```c
/* hold down 50ms to make sure the address available */
k_sleep(K_MSEC(50));
```

Change it to `K_MSEC(200)`
