// Copyright 2024 Andrew Smith (@Andrew-William-Smith) ::::::::: SOLI DEO GLORIA
// SPDX-License-Identifier: GPL-2.0-or-later

#include "mini_key_override.h"

bool mini_key_override_process(
    uint16_t keycode,
    const keyrecord_t *record,
    const struct mini_key_override *overrides,
    uint8_t override_count)
{
  // This implementation is derived in part from
  // https://getreuer.info/posts/keyboards/custom-shift-keys/index.html.
  static uint16_t override_current = KC_NO;

  // Ensure that only one override is active at a time. If the user is releasing
  // an overridden key or holding it when another overridden key is pressed,
  // cancel the effect of the previous override.
  if (override_current != KC_NO) {
    unregister_code16(override_current);
    override_current = KC_NO;
  }

  // Only respond to key-press events: defer to default processing for all other
  // events, including key releases.
  if (!record->event.pressed) { return true; }

  // Fall back to the default key behaviour if this is a Mod-Tap or Tap-Hold key
  // being held.
  if ((IS_QK_MOD_TAP(keycode) || IS_QK_LAYER_TAP(keycode))
      && record->tap.count == 0) {
    return true;
  }

  const uint8_t mods_current = get_mods() | get_weak_mods();
  for (uint8_t i = 0; i < override_count; ++i, ++overrides) {
    // For an override to take effect, the free keycode must match, all target
    // modifiers must be held, and none of the modifiers in the supression list
    // can be held.
    if (QK_MODS_GET_BASIC_KEYCODE(keycode) != overrides->keycode_trigger) {
      continue;
    }

    if (mods_current) {
      if (!(mods_current & overrides->modifier_mask)
          || (mods_current & overrides->modifier_mask_suppress)) {
        continue;
      }
      override_current = overrides->keycode_held;
    } else {
      override_current = overrides->keycode_free;
    }

    // Clear all modifiers, press the target key, and restore modifiers.
    clear_mods();
    register_code16(override_current);
    set_mods(mods_current);
    return false;
  }

  return true;
}
