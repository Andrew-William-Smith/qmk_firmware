// Copyright 2024 Andrew Smith (@Andrew-William-Smith) ::::::::: SOLI DEO GLORIA
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#include "mini_key_override.h"

enum hotdox_layer {
  HOTDOX_LAYER_QWERTY = 0,
  HOTDOX_LAYER_NAV = 1,
  HOTDOX_LAYER_NUM = 2,
};

enum hotdox_keycode {
  HD_DCLN = SAFE_RANGE,
  HD_RARR,
  HD_I8,
  HD_I16,
  HD_I32,
  HD_I64,
  HD_SIZE,
  HD_STD,
  HD_000,
  HD_HEX,

  // Custom keycodes for sending Slack reactions.
  HD_ACK,  // :ack:
  HD_THUP, // :+1:
  HD_THX,  // :thanks:
  HD_NP,   // :np:
  HD_SAD,  // :sadpanda:

  // Custom keycodes due to // https://github.com/qmk/qmk_firmware/issues/17569.
  // These keycodes are aliased to basic keycodes as non-basic keycodes are
  // incompatible with layer tap.
  HD_XLPN = KC_INT1,
  HD_XRPN = KC_INT2,
  HD_XLBR = KC_INT3,
  HD_XRBR = KC_INT4,
  HD_XUND = KC_INT5,
  HD_XTLD = KC_INT6,
  HD_XPIP = KC_INT7,
  HD_MCLK = KC_INT8,
  HD_MNAV = KC_INT9,
};

#define HD_LPRN LT(HOTDOX_LAYER_NUM, HD_XLPN)
#define HD_RPRN LT(HOTDOX_LAYER_NUM, HD_XRPN)
#define HD_LBRC LT(HOTDOX_LAYER_NAV, HD_XLBR)
#define HD_RBRC LT(HOTDOX_LAYER_NAV, HD_XRBR)
#define HD_LOCK LGUI(LCTL(KC_TAB))
#define HD_ATAB LALT(KC_TAB)
#define HD_STAB LSFT(KC_TAB)
#define HD_MONV MO(HOTDOX_LAYER_NAV)
#define HD_LKNU TO(HOTDOX_LAYER_NUM)
#define HD_ULNU TO(HOTDOX_LAYER_QWERTY)

static const struct mini_key_override hd_key_overrides[] = {
  // In order to make writing code in nearly all programming languages a bit
  // more convenient, use dedicated keys for parentheses with curly braces as
  // shifted pairs.
  { HD_XLPN, KC_LPRN, KC_LCBR,      MOD_MASK_SHIFT, 0 },
  { HD_XRPN, KC_RPRN, KC_RCBR,      MOD_MASK_SHIFT, 0 },
  // Now let's address the last remaining paired delimiters: square brackets are
  // assigned unshifted keys, with the single and double quote characters as
  // their shifted pairs.
  { HD_XLBR, KC_LBRC, KC_DQUO,      MOD_MASK_SHIFT, 0 },
  { HD_XRBR, KC_RBRC, KC_QUOT,      MOD_MASK_SHIFT, 0 },
  // Unless I'm writing Lisp, I type significantly more underscores than dashes:
  // as such, promote underscore to the unshifted key, with dash being shifted.
  { HD_XUND, KC_UNDS, KC_MINS,      MOD_MASK_SHIFT, 0 },
  // I really only type grave accents when writing shell scripts, and even then
  // sparingly: swap the grave accent and the tilde, which I type much more
  // frequently.
  { HD_XTLD, KC_TILD, KC_GRV,       MOD_MASK_SHIFT, 0 },
  // The same applies for pipe and backslash.
  { HD_XPIP, KC_PIPE, KC_BSLS,      MOD_MASK_SHIFT, 0 },

  // Make Delete a shifted pair of Backslash. I really only use either in non-
  // code environments, so placing Delete on an unshifted key isn't particularly
  // important.
  { KC_BSPC, KC_BSPC, KC_DEL,       MOD_MASK_SHIFT, 0 },

  // The combination Shift+Space locks the screen under macOS.
  { KC_SPC,  KC_SPC,  HD_LOCK,      MOD_MASK_SHIFT, 0 },

  // The mouse-click key sends a left click when no modifiers are held, a right
  // click when Shift is held, and a middle click when Control is held.
  { HD_MCLK, KC_BTN1, KC_BTN2,      MOD_MASK_SHIFT, MOD_MASK_CTRL },
  { HD_MCLK, KC_BTN1, KC_BTN3,      MOD_MASK_CTRL,  MOD_MASK_SHIFT },
  // Additionally, define a key that sends mouse buttons 4 and 5, which most
  // applications seem to interpret as backward and forward navigation,
  // respectively.
  { HD_MNAV, KC_BTN4, KC_BTN5,      MOD_MASK_SHIFT, 0 },

  // When Control is held, the Volume Up and Down keys should send Previous and
  // Next Track, respectively.
  { KC_VOLD, KC_VOLD, KC_MPRV,      MOD_MASK_CTRL, MOD_MASK_SHIFT },
  { KC_VOLU, KC_VOLU, KC_MNXT,      MOD_MASK_CTRL, MOD_MASK_SHIFT },
  // When Shift is held, these keys should send Alt+Shift+the target key for
  // fine-grained volume control on macOS.
  { KC_VOLD, KC_VOLD, LSA(KC_VOLD), MOD_MASK_SHIFT, MOD_MASK_CTRL },
  { KC_VOLU, KC_VOLU, LSA(KC_VOLU), MOD_MASK_SHIFT, MOD_MASK_CTRL },
  // When Play/Pause is pressed when Shift is held, mute audio.
  { KC_MPLY, KC_MPLY, KC_MUTE,      MOD_MASK_SHIFT, 0 },
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
     * │ ~ `  │ ! │ @ │ # │ $ │ % │   │                     │KLK│ ^ │ & │ * │ 0 │ 0 │ WCaps│
     * ├──┬───┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──┬───┤
     * │_-┊Hyp│ Q │ W │ E │ R │ T │LSu│                     │RSu│ Y │ U │ I │ O │ P │=+┊Hyp│
     * ├──┼───┼───┼───┼───┼───┼───┤Tab│                     │| \├───┼───┼───┼───┼───┼──┼───┤
     * │["┊LNV│ A │ S │ D │ F │ G ├───┤                     ├───┤ H │ J │ K │ L │ ; │]'┊LNV│
     * ├──┼───┼───┼───┼───┼───┼───┤Lck│                     │Del├───┼───┼───┼───┼───┼──┼───┤
     * │({┊LNU│ Z │ X │ C │ V │ B │Spc│                     │Bsp│ N │ M │ , │ . │ / │)}┊LNU│
     * └──┼───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼──┴┬──┘
     *    │ ( │Alt│STb│Tab│Alt│         │   │Ply│ │VDn│VUp│         │LtA│DnA│UpA│RtA│ ) │
     *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
     *                              │LSh│LCt│Hom│ │PgU│RCt│RSh│
     *                              ├┄┄┄┼┄┄┄┼───┤ ├───┼┄┄┄┼┄┄┄┤
     *                              │Spc│Ent│End│ │PgD│Esc│Bsp│
     *                              └───┴───┴───┘ └───┴───┴───┘ */
    [HOTDOX_LAYER_QWERTY] = LAYOUT_ergodox_pretty(
        HD_XTLD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______,            HD_LKNU, KC_CIRC, KC_AMPR, KC_ASTR, KC_0,    KC_0,    CW_TOGG,
 HYPR_T(HD_XUND),KC_Q,    KC_W,    KC_E,    KC_R,    KC_T, LGUI_T(KC_TAB),RGUI_T(HD_XPIP), KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    HYPR_T(KC_EQL),
        HD_LBRC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                 KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, HD_RBRC,
        HD_LPRN, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,     KC_SPC,            KC_BSPC, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, HD_RPRN,
        HD_LPRN, KC_RALT, HD_STAB, KC_TAB,  KC_RALT,                                                KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, HD_RPRN,
                                                     _______, KC_MPLY,            KC_VOLD, KC_VOLU,
                                                              KC_HOME,            KC_PGUP,
                               LSFT_T(KC_SPC), LCTL_T(KC_ENT), KC_END,            KC_PGDN, RCTL_T(KC_ESC), RSFT_T(KC_BSPC)
    ),
    /* ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
     * │ +1   │F1 │F2 │F3 │F4 │F5 │F11│                     │F12│F6 │F7 │F8 │F9 │F10│      │
     * ├──────┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──────┤
     * │ ACK  │i8 │i16│i32│i64│sz │   │                     │   │Hom│PgD│PgU│End│   │      │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │THANKS│MLt│MDn│MUp│MRt│MCl├───┤                     ├───┤LtA│DnA│UpA│RtA│:: │      │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │ NP   │WLt│WDn│WUp│WRt│MNv│   │                     │   │   │ATb│   │-> │std│      │
     * └──┬───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼───┬──┘
     *    │SAD│   │   │   │   │         │   │   │ │   │   │         │   │   │   │   │   │
     *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
     *                              │   │   │   │ │   │   │   │
     *                              │MCl│   ├───┤ ├───┤   │   │
     *                              │   │   │   │ │   │   │   │
     *                              └───┴───┴───┘ └───┴───┴───┘ */
    [HOTDOX_LAYER_NAV] = LAYOUT_ergodox_pretty(
        HD_THUP, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F11,             KC_F12,  KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
        HD_ACK,  HD_I8,   HD_I16,  HD_I32,  HD_I64,  HD_SIZE, _______,            _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______,
        HD_THX,  KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, HD_MCLK,                              KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, HD_DCLN, _______,
        HD_NP,   KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, HD_MNAV, _______,            _______, _______, HD_ATAB, _______, HD_RARR, HD_STD,  _______,
        HD_SAD,  _______, _______, _______, _______,                                                _______, _______, _______, _______, _______,
                                                     _______, _______,            _______, _______,
                                                              _______,            _______,
                                            HD_MCLK, _______, _______,            _______, _______, _______
    ),

    /* ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
     * │      │   │   │   │   │   │   │                     │ULk│ - │ * │ / │ ' │ _ │ 0x   │
     * ├──────┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   │   │                     │   │ C │ 7 │ 8 │ 9 │ F │ +    │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │Tab├───┼───┼───┼───┼───┼──────┤
     * │ LNAV │ ( │ % │F4 │ ^ │ ) ├───┤                     ├───┤ B │ 4 │ 5 │ 6 │ E │ Ent  │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   │   │                     │ = │ A │ 1 │ 2 │ 3 │ D │      │
     * └──┬───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼───┬──┘
     *    │   │   │   │   │   │         │   │   │ │   │   │         │ 0 │ 0 │ . │000│ , │
     *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
     *                              │   │   │   │ │   │   │   │
     *                              │   │   ├───┤ ├───┤   │   │
     *                              │   │   │   │ │   │   │   │
     *                              └───┴───┴───┘ └───┴───┴───┘ */
    [HOTDOX_LAYER_NUM] = LAYOUT_ergodox_pretty(
        _______, _______, _______, _______, _______, _______, _______,            HD_ULNU, KC_PMNS, KC_PAST, KC_PSLS, KC_QUOT, KC_UNDS, HD_HEX,
        _______, _______, _______, _______, _______, _______, _______,            KC_TAB,  S(KC_C), KC_P7,   KC_P8,   KC_P9,   S(KC_F), KC_PPLS,
        HD_MONV, KC_LPRN, KC_PERC, KC_F4,   KC_CIRC, KC_RPRN,                              S(KC_B), KC_P4,   KC_P5,   KC_P6,   S(KC_E), KC_PENT,
        _______, _______, _______, _______, _______, _______, _______,            KC_EQL,  S(KC_A), KC_P1,   KC_P2,   KC_P3,   S(KC_D), _______,
        _______, _______, _______, _______, _______,                                                KC_P0,   KC_P0,   KC_PDOT, HD_000,  KC_COMM,
                                                     _______, _______,            _______, _______,
                                                              _______,            _______,
                                            _______, _______, _______,            _______, _______, _______
    ),

    ///* ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
    // * │      │   │   │   │   │   │   │                     │   │   │   │   │   │   │      │
    // * ├──────┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──────┤
    // * │      │   │   │   │   │   │   │                     │   │   │   │   │   │   │      │
    // * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
    // * │      │   │   │   │   │   ├───┤                     ├───┤   │   │   │   │   │      │
    // * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
    // * │      │   │   │   │   │   │   │                     │   │   │   │   │   │   │      │
    // * └──┬───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼───┬──┘
    // *    │   │   │   │   │   │         │   │   │ │   │   │         │   │   │   │   │   │
    // *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
    // *                              │   │   │   │ │   │   │   │
    // *                              │   │   ├───┤ ├───┤   │   │
    // *                              │   │   │   │ │   │   │   │
    // *                              └───┴───┴───┘ └───┴───┴───┘ */
    //[HOTDOX_LAYER_NAME] = LAYOUT_ergodox_pretty(
    //    _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______, _______, _______,
    //    _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______, _______, _______,
    //    _______, _______, _______, _______, _______, _______,                              _______, _______, _______, _______, _______, _______,
    //    _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______, _______, _______,
    //    _______, _______, _______, _______, _______,                                                _______, _______, _______, _______, _______,
    //                                                 _______, _______,            _______, _______,
    //                                                          _______,            _______,
    //                                        _______, _______, _______,            _______, _______, _______
    //),
};

void keyboard_post_init_user(void)
{
  // We do not use the RGB Light feature, as this keyboard does not feature any
  // under-key LEDs.
}

#define HD_SEND_STRING(KEYCODE, STR)                 \
  case (KEYCODE): {                                  \
    if (record->event.pressed) { SEND_STRING(STR); } \
    break;                                           \
  }

#define HD_SEND_PREFIX_STRING(KEYCODE, PREFIX, STR)       \
  case (KEYCODE): {                                       \
    if (record->event.pressed) {                          \
      if (mods & MOD_MASK_SHIFT) { SEND_STRING(PREFIX); } \
      SEND_STRING(STR);                                   \
    }                                                     \
    break;                                                \
  }

#define HD_SEND_SLACK_STRING(KEYCODE, STR)                               \
  case (KEYCODE): {                                                      \
    if (!record->event.pressed) { break; }                               \
    if (mods & MOD_MASK_SHIFT) {                                         \
      /* React to a message rather than sending an independent reply. */ \
      SEND_STRING(SS_LGUI(SS_LSFT(SS_TAP(X_BSLS))) SS_DELAY(500));       \
    }                                                                    \
    SEND_STRING(STR);                                                    \
    if (mods & MOD_MASK_SHIFT) { SEND_STRING(SS_TAP(X_ENT)); }           \
    break;                                                               \
  }

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  static bool caps_word_enabled = false;

  if (!mini_key_override_process(
        keycode, record, hd_key_overrides,
        sizeof(hd_key_overrides) / sizeof(struct mini_key_override))) {
    // Disable Caps Word if an override for a key other than underscore or
    // backspace was invoked. We have to set this state manually since we are
    // not passing the key event beyond this callback.
    const uint16_t base_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
    if (base_keycode != HD_XUND && base_keycode != KC_BSPC) { caps_word_off(); }
    return false;
  }

  // Only send an LED control request if the Caps Word state has changed since
  // the last invocation of this callback.
  const bool new_caps_word = is_caps_word_on();
  if (caps_word_enabled != new_caps_word) {
    caps_word_enabled = new_caps_word;
    if (caps_word_enabled) {
      ergodox_right_led_2_on();
    } else {
      ergodox_right_led_2_off();
    }
  }

  // Ensure that Shift being held will not interfere with the strings we plan to
  // send.
  const uint8_t mods = get_mods();
  if (mods & MOD_MASK_SHIFT) { del_mods(MOD_MASK_SHIFT); }

  bool handle_default = false;
  switch (keycode) {
    HD_SEND_STRING(HD_DCLN, "::")
    HD_SEND_STRING(HD_SIZE, "size_t")
    HD_SEND_STRING(HD_STD,  "std::")
    HD_SEND_STRING(HD_000,  "000")
    HD_SEND_STRING(HD_HEX,  "0x")

    HD_SEND_PREFIX_STRING(HD_I8,  "u", "int8_t")
    HD_SEND_PREFIX_STRING(HD_I16, "u", "int16_t")
    HD_SEND_PREFIX_STRING(HD_I32, "u", "int32_t")
    HD_SEND_PREFIX_STRING(HD_I64, "u", "int64_t")

    HD_SEND_SLACK_STRING(HD_ACK,  ":ack:")
    HD_SEND_SLACK_STRING(HD_THUP, ":+1:")
    HD_SEND_SLACK_STRING(HD_THX,  ":thanks:")
    HD_SEND_SLACK_STRING(HD_NP,   ":np:")
    HD_SEND_SLACK_STRING(HD_SAD,  ":sadpanda:")

    case HD_RARR: {
      if (!record->event.pressed) { break; }

      // Make this macro useful for all languages including arrow-like tokens.
      if (mods & MOD_MASK_SHIFT) {
        SEND_STRING("=>");
      } else {
        SEND_STRING("->");
      }
      break;
    }

    default: {
      handle_default = true;
      break;
    }
  }

  set_mods(mods);
  return handle_default;
}

#undef HD_SEND_SLACK_STRING
#undef HD_SEND_PREFIX_STRING
#undef HD_SEND_STRING

layer_state_t layer_state_set_user(layer_state_t state)
{
  switch (get_highest_layer(state)) {
    case 1: {
      // Illuminate the yellow LED upon activating the numeric layer.
      ergodox_right_led_1_off();
      ergodox_right_led_3_on();
      break;
    }
    case 2: {
      // Illuminate the red LED upon activating the numeric layer.
      ergodox_right_led_3_off();
      ergodox_right_led_1_on();
      break;
    }
    case 0:
    default: {
      ergodox_right_led_1_off();
      ergodox_right_led_3_off();
      break;
    }
  }

  return state;
}
