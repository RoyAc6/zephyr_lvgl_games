# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

if(CONFIG_BOARD_FRDM_MCXN947)
    set(SHIELD lcd_par_s035_8080)
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(brick_breaker)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})
