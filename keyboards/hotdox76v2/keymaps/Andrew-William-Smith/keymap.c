// Copyright 2021 JasonRen(biu)
// Copyright 2022 Drashna Jael're (@Drashna Jael're)
// Copyright 2024 Andrew Smith (@Andrew-William-Smith) ::::::::: SOLI DEO GLORIA
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include <string.h>
#include <transactions.h>

#include "bongo_cat.inc"
#include "cistercian.h"

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
  HD_STD,
  HD_000,
  HD_HEX,
  HD_CNT,
};

// The Volume Down and Up keys send Previous and Next Track, respectively, when
// Control is held.
static const key_override_t hd_override_prev_track
    = ko_make_with_layers_negmods_and_options(
        MOD_MASK_CTRL,  // When Control is held
        KC_VOLD,        // along with Volume Down,
        KC_MPRV,        // send Previous Track
        ~0,             // on any layer
        MOD_MASK_SHIFT, // unless Shift is also held.
        ko_option_no_reregister_trigger);
static const key_override_t hd_override_next_track
    = ko_make_with_layers_negmods_and_options(
        MOD_MASK_CTRL, KC_VOLU, KC_MNXT, ~0, MOD_MASK_SHIFT,
        ko_option_no_reregister_trigger);

// Additionally, they send Alt+Shift+the target key when Shift is held. These
// key combinations allow for fine-grained volume control on macOS.
static const key_override_t hd_override_volume_down_fine
    = ko_make_with_layers_negmods_and_options(
        MOD_MASK_SHIFT, KC_VOLD, LSA(KC_VOLD), ~0, MOD_MASK_CTRL,
        ko_option_no_reregister_trigger);
static const key_override_t hd_override_volume_up_fine
    = ko_make_with_layers_negmods_and_options(
        MOD_MASK_SHIFT, KC_VOLU, LSA(KC_VOLU), ~0, MOD_MASK_CTRL,
        ko_option_no_reregister_trigger);

// When Play/Pause is pressed when Shift is held, mute audio.
static const key_override_t hd_override_mute
    = ko_make_basic(MOD_MASK_SHIFT, KC_MPLY, KC_MUTE);

// In order to make writing code in nearly all programming languages a bit more
// convenient, use dedicated keys for parentheses with curly braces as shifted
// pairs for the C family.
static const key_override_t hd_override_lcbr
    = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_LCBR);
static const key_override_t hd_override_rcbr
    = ko_make_basic(MOD_MASK_SHIFT, KC_RPRN, KC_RCBR);

// Since curly braces are already handled, we can make backslash and pipe
// shifted pairs of the square brackets.
static const key_override_t hd_override_backslash
    = ko_make_basic(MOD_MASK_SHIFT, KC_LBRC, KC_BSLS);
static const key_override_t hd_override_pipe
    = ko_make_basic(MOD_MASK_SHIFT, KC_RBRC, KC_PIPE);

// Unless I'm writing Lisp, I type significantly more underscores than dashes:
// as such, promote underscore to the unshifted key, with dash being shifted.
static const key_override_t hd_override_dash
    = ko_make_basic(MOD_MASK_SHIFT, KC_UNDS, KC_MINS);

const key_override_t **key_overrides = (const key_override_t *[]){
    &hd_override_prev_track,
    &hd_override_next_track,
    &hd_override_volume_down_fine,
    &hd_override_volume_up_fine,
    &hd_override_mute,
    &hd_override_lcbr,
    &hd_override_rcbr,
    &hd_override_backslash,
    &hd_override_pipe,
    &hd_override_dash,
    NULL,
};

#define HD_LNAV MO(HOTDOX_LAYER_NAV)
#define HD_LNUM MO(HOTDOX_LAYER_NUM)
#define HD_LOCK LGUI(LCTL(KC_TAB))
#define HD_STAB LSFT(KC_TAB)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
     * │ ` ~  │ 1 │ 2 │ 3 │ 4 │ 5 │Cnt│                     │Lck│ 6 │ 7 │ 8 │ 9 │ 0 │ WCaps│
     * ├──────┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──────┤
     * │ _ -  │ Q │ W │ E │ R │ T │ \ │                     │ | │ Y │ U │ I │ O │ P │  = + │
     * ├──────┼───┼───┼───┼───┼───┤ [ │                     │ ] ├───┼───┼───┼───┼───┼──────┤
     * │L_NAV │ A │ S │ D │ F │ G ├───┤                     ├───┤ H │ J │ K │ L │ ; │  ' " │
     * ├──────┼───┼───┼───┼───┼───┤LSu│                     │RSu├───┼───┼───┼───┼───┼──────┤
     * │ ( {  │ Z │ X │ C │ V │ B │Tab│                     │Del│ N │ M │ , │ . │ / │  ) } │
     * └──┬───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼───┬──┘
     *    │ ( │Alt│STb│Tab│Alt│         │Rgb│Ply│ │VDn│VUp│         │LtA│DnA│UpA│RtA│ ) │
     *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
     *                              │LSh│LCt│Hom│ │PgU│RCt│RSh│
     *                              │~~~│~~~├───┤ ├───┤~~~│~~~│
     *                              │Spc│Ent│End│ │PgD│Esc│Bsp│
     *                              └───┴───┴───┘ └───┴───┴───┘ */
    [HOTDOX_LAYER_QWERTY] = LAYOUT_ergodox_pretty(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    HD_CNT,             HD_LOCK, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    CW_TOGG,
        KC_UNDS, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_LBRC,            KC_RBRC, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
        HD_LNAV, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                 KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LPRN, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B, LGUI_T(KC_TAB), RGUI_T(KC_DEL), KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RPRN,
        KC_LPRN, KC_RALT, HD_STAB, KC_TAB,  KC_RALT,                                                KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_RPRN,
                                                     RGB_MOD, KC_MPLY,            KC_VOLD, KC_VOLU,
                                                              KC_HOME,            KC_PGUP,
                               LSFT_T(KC_SPC), LCTL_T(KC_ENT), KC_END,            KC_PGDN, RCTL_T(KC_ESC), RSFT_T(KC_BSPC)
    ),
    /* ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
     * │      │   │   │   │   │   │   │                     │   │   │i8 │i16│i32│i64│      │
     * ├──────┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   │   │                     │   │Hom│PgD│PgU│End│   │      │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   ├───┤                     ├───┤LtA│DnA│UpA│RtA│:: │      │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   │   │                     │   │   │   │   │-> │std│      │
     * └──┬───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼───┬──┘
     *    │   │   │   │   │   │         │   │   │ │   │   │         │   │   │   │   │   │
     *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
     *                              │   │   │   │ │   │   │   │
     *                              │L_N│   ├───┤ ├───┤   │   │
     *                              │   │   │   │ │   │   │   │
     *                              └───┴───┴───┘ └───┴───┴───┘ */
    [HOTDOX_LAYER_NAV] = LAYOUT_ergodox_pretty(
        _______, _______, _______, _______, _______, _______, _______,            _______, _______, HD_I8,   HD_I16,  HD_I32,  HD_I64,  _______,
        _______, _______, _______, _______, _______, _______, _______,            _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______, _______,
        _______, _______, _______, _______, _______, _______,                              KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, HD_DCLN, _______,
        _______, _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______, HD_RARR, HD_STD,  _______,
        _______, _______, _______, _______, _______,                                                _______, _______, _______, _______, _______,
                                                     _______, _______,            _______, _______,
                                                              _______,            _______,
                                            HD_LNUM, _______, _______,            _______, _______, _______
    ),

    /* ┌──────┬───┬───┬───┬───┬───┬───┐                     ┌───┬───┬───┬───┬───┬───┬──────┐
     * │      │   │   │   │   │   │   │                     │ = │ - │ * │ / │ ' │ _ │NumLk │
     * ├──────┼───┼───┼───┼───┼───┼───┤                     ├───┼───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   │   │                     │   │ 7 │ 8 │ 9 │ C │ F │      │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │ + ├───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   ├───┤                     ├───┤ 4 │ 5 │ 6 │ B │ E │      │
     * ├──────┼───┼───┼───┼───┼───┤   │                     │   ├───┼───┼───┼───┼───┼──────┤
     * │      │   │   │   │   │   │   │                     │Ent│ 1 │ 2 │ 3 │ A │ D │      │
     * └──┬───┼───┼───┼───┼───┼───┴───┘ ┌───┬───┐ ┌───┬───┐ └───┴───┼───┼───┼───┼───┼───┬──┘
     *    │   │   │   │   │   │         │   │   │ │   │   │         │ 0 │ . │000│0x │ , │
     *    └───┴───┴───┴───┴───┘     ┌───┼───┼───┤ ├───┼───┼───┐     └───┴───┴───┴───┴───┘
     *                              │   │   │   │ │   │   │   │
     *                              │   │   ├───┤ ├───┤   │   │
     *                              │   │   │   │ │   │   │   │
     *                              └───┴───┴───┘ └───┴───┴───┘ */
    [HOTDOX_LAYER_NUM] = LAYOUT_ergodox_pretty(
        _______, _______, _______, _______, _______, _______, _______,            KC_EQL,  KC_PMNS, KC_PAST, KC_PSLS, KC_QUOT, KC_UNDS, KC_NUM,
        _______, _______, _______, _______, _______, _______, _______,            KC_PPLS, KC_P7,   KC_P8,   KC_P9,   S(KC_C), S(KC_F), _______,
        _______, _______, _______, _______, _______, _______,                              KC_P4,   KC_P5,   KC_P6,   S(KC_B), S(KC_E), _______,
        _______, _______, _______, _______, _______, _______, _______,            KC_PENT, KC_P1,   KC_P2,   KC_P3,   S(KC_A), S(KC_D), _______,
        _______, _______, _______, _______, _______,                                                KC_P0,   KC_PDOT, HD_000,  HD_HEX,  KC_COMM,
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

struct __attribute__((packed)) hotdox_state_left {
  uint8_t pad;
};

struct __attribute__((packed)) hotdox_state_right {
  uint16_t total_keypresses;
  uint8_t keys_held;
};

union __attribute__((packed)) hotdox_state {
  struct hotdox_state_left left;
  struct hotdox_state_right right;
};

static struct hotdox_state_left state_left;
static struct hotdox_state_right state_right;

// TODO(awsmith): Determine why this counter can't be placed in state_left.
static uint16_t state_counter = 0;

void hotdox_sync_handler(
    uint8_t in_bytes, const void *in_data,
    __attribute__((unused)) uint8_t out_bytes,
    __attribute__((unused)) void *out_data)
{
  const union hotdox_state *state_in = in_data;

  if (is_keyboard_left()) {
    memcpy(&state_left, &state_in->left, sizeof(struct hotdox_state_left));
  } else {
    memcpy(&state_right, &state_in->right, sizeof(struct hotdox_state_right));
  }
}

void keyboard_post_init_user(void)
{
  transaction_register_rpc(HOTDOX_SYNC, hotdox_sync_handler);
  memset(&state_left, 0, sizeof(struct hotdox_state_left));
  memset(&state_right, 0, sizeof(struct hotdox_state_right));
}

void housekeeping_task_user(void)
{
  // Periodically sync the portion of the global keyboard state required by the
  // slave from the master, retrying on the next housekeeping tick if the
  // transaction fails.
  static uint32_t ts_last_sync = 0;
  if (!is_keyboard_master() || timer_elapsed32(ts_last_sync) < 50) { return; }

  if ((is_keyboard_left() && transaction_rpc_exec(
         HOTDOX_SYNC, sizeof(struct hotdox_state_right), &state_right,
         sizeof(struct hotdox_state_left), &state_left)) ||
      (!is_keyboard_left() && transaction_rpc_exec(
         HOTDOX_SYNC, sizeof(struct hotdox_state_left), &state_left,
         sizeof(struct hotdox_state_right), &state_right))) {
    ts_last_sync = timer_read32();
  }
}

#define HD_SEND_STRING(KEYCODE, STR)                 \
  case (KEYCODE): {                                  \
    if (record->event.pressed) { SEND_STRING(STR); } \
    return false;                                    \
  }

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  if (is_keyboard_master()) {
    if (record->event.pressed) {
      state_right.total_keypresses
          = (state_right.total_keypresses + 1) % CISTERCIAN_MODULO;
      state_right.keys_held++;
    } else {
      state_right.keys_held--;
    }
  }

  switch (keycode) {
    HD_SEND_STRING(HD_DCLN, "::")
    HD_SEND_STRING(HD_RARR, "->")
    HD_SEND_STRING(HD_I8,   "int8_t")
    HD_SEND_STRING(HD_I16,  "int16_t")
    HD_SEND_STRING(HD_I32,  "int32_t")
    HD_SEND_STRING(HD_I64,  "int64_t")
    HD_SEND_STRING(HD_STD,  "std::")
    HD_SEND_STRING(HD_000,  "000")
    HD_SEND_STRING(HD_HEX,  "0x")

    case HD_CNT: {
      if (!record->event.pressed) { return false; }

      // Key overrides do not work with custom keycodes, so we have to check
      // which modifiers are active manually.
      const uint8_t mods = get_mods();
      if (mods & MOD_MASK_CTRL) {
        state_counter = 0;
      } else if (mods & MOD_MASK_SHIFT) {
        state_counter--;
      } else {
        state_counter++;
      }
      return false;
    }

    default: { return true; }
  }
}

#undef HD_SEND_STRING

bool oled_task_user(void)
{
  static uint32_t bongo_cat_last_keypress = 0;
  static uint32_t bongo_cat_last_idle = 0;
  static uint8_t bongo_cat_idle_frame = 0;
  static uint16_t state_counter_last = 0;

  if (is_keyboard_left()) {
    if (state_counter != state_counter_last) {
      state_counter_last = state_counter;
      oled_clear();
      if (state_counter) {
        cistercian_render(state_counter, 0, 127 - CISTERCIAN_CANVAS_SIZE);
      }
      oled_render_dirty(true);
    }
  } else {
    // Render the current frame of the Bongo Cat animation. This is why I've
    // gone to great pains to reduce IMEM usage everywhere else. :)
    if (state_right.keys_held) {
      oled_write_raw_P(
          bongo_cat_tap[state_right.total_keypresses % BONGO_CAT_TAP_FRAMES],
          BONGO_CAT_FRAME_BYTES);
      bongo_cat_last_keypress = timer_read32();

      // Reset the animation for a smoother transition out of the prep state.
      bongo_cat_idle_frame = 0;
    } else if (
        timer_elapsed32(bongo_cat_last_keypress) <= BONGO_CAT_PREP_TIMEOUT_MS) {
      oled_write_raw_P(bongo_cat_prep[0], BONGO_CAT_FRAME_BYTES);
    } else {
      oled_write_raw_P(
          bongo_cat_idle[bongo_cat_idle_frame % BONGO_CAT_IDLE_FRAMES],
          BONGO_CAT_FRAME_BYTES);

      // Step the animation if the current frame has expired.
      if (timer_elapsed32(bongo_cat_last_idle) > BONGO_CAT_FRAME_DURATION_MS) {
        bongo_cat_last_idle = timer_read32();
        bongo_cat_idle_frame++;
      }
    }

    cistercian_render(state_right.total_keypresses, 0, 2);
  }

  return false;
}

// Even though we are not using magic keycodes, there are some vestigial
// magic-related functions still left in the default image. Override these with
// stubs to save some IMEM.
uint16_t keycode_config(uint16_t keycode) { return keycode; }
uint8_t mod_config(uint8_t mod) { return mod; }
