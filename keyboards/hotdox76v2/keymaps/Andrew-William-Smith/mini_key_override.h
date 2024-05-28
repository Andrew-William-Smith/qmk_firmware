// Copyright 2024 Andrew Smith (@Andrew-William-Smith) ::::::::: SOLI DEO GLORIA
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MINI_KEY_OVERRIDE_H_INCLUDED
#define MINI_KEY_OVERRIDE_H_INCLUDED

#include QMK_KEYBOARD_H

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

/// A key override definition, defining the keycodes to be sent when the
/// modifiers are free versus held. If any modifiers in the suppression mask are
/// also held, we shall not consider this override record to have matched. Due
/// to a bug in QMK (https://github.com/qmk/qmk_firmware/issues/17569),
/// overridden keys must be registered using custom keycodes.
struct __attribute__((packed)) mini_key_override {
  uint16_t keycode_trigger;
  uint16_t keycode_free;
  uint16_t keycode_held;
  uint8_t modifier_mask;
  uint8_t modifier_mask_suppress;
};

/// Process the specified set of overrides for the specified keycode. Returns
/// true if standard key processing should continue or false if the key was
/// intercepted and handled as an override.
bool mini_key_override_process(
    uint16_t keycode,
    const keyrecord_t *record,
    const struct mini_key_override *overrides,
    uint8_t override_count);

#endif // MINI_KEY_OVERRIDE_H_INCLUDED
