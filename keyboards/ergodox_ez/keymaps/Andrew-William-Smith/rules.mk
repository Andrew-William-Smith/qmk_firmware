# Copyright 2024 Andrew Smith (@Andrew-William-Smith) :::::::::: SOLI DEO GLORIA
# SPDX-License-Identifier: GPL-2.0-or-later

# Enable some additional features used in the keymap.
CAPS_WORD_ENABLE=yes
LTO_ENABLE=yes
MOUSEKEY_ENABLE=yes
NKRO_ENABLE=yes
SEND_STRING_ENABLE=yes

# Disable unused functionality to free up some IMEM on our piddly AVR.
AVR_USE_MINIMAL_PRINTF=yes
BOOTMAGIC_ENABLE=no
CONSOLE_ENABLE=no
COMMAND_ENABLE=no
DEBUG_ENABLE=no
GRAVE_ESC_ENABLE=no
KEY_OVERRIDE_ENABLE=no
MAGIC_ENABLE=no
MUSIC_ENABLE=no
RGBLIGHT_ENABLE=no
SPACE_CADET_ENABLE=no
SWAP_HANDS_ENABLE=no
TERMINAL_ENABLE=no

SRC += mini_key_override.c
