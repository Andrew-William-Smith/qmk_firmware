// Copyright 2024 Andrew Smith (@Andrew-William-Smith) ::::::::: SOLI DEO GLORIA
// SPDX-License-Identifier: GPL-2.0-or-later

// Tap-Hold options.
#define HOLD_ON_OTHER_KEY_PRESS
#define QUICK_TAP_TERM 100

// Mouse-key options. We must undefine each option, as most are overridden in
// the keyboard-level config.h.
#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY 0
#undef MOUSEKEY_INTERVAL
#define MOUSEKEY_INTERVAL 16
#undef MOUSEKEY_MAX_SPEED
#define MOUSEKEY_MAX_SPEED 4
#undef MOUSEKEY_TIME_TO_MAX
#define MOUSEKEY_TIME_TO_MAX 64
#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY 0
#undef MOUSEKEY_WHEEL_MAX_SPEED
#define MOUSEKEY_WHEEL_MAX_SPEED 4
#undef MOUSEKEY_WHEEL_TIME_TO_MAX
#define MOUSEKEY_WHEEL_TIME_TO_MAX 64

// I don't really see how I could ever use more than 8 layers: disable the use
// of any more to save some IMEM.
#define LAYER_STATE_8BIT

// Disable unused functionality to save IMEM.
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define NO_ACTION_FUNCTION
#define NO_ACTION_MACRO
#define NO_ACTION_ONESHOT
#define NO_MUSIC_MODE

// Disable debugging: remove this block if debugging functionality is desired.
#ifndef NO_DEBUG
#  define NO_DEBUG
#endif
#if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
#  define NO_PRINT
#endif
