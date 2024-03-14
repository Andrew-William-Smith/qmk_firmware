// Copyright 2024 Andrew Smith (@Andrew-William-Smith) ::::::::: SOLI DEO GLORIA
// SPDX-License-Identifier: GPL-2.0-or-later

#include "cistercian.h"

#include QMK_KEYBOARD_H
#include <inttypes.h>
#include "progmem.h"

#define CISTERCIAN_STROKE_ROW_ORIGIN_MASK 0x80
#define CISTERCIAN_STROKE_COL_ORIGIN_MASK 0x40
#define CISTERCIAN_STROKE_DELTA_MASK      0x03

/// The individual strokes that comprise a Cistercian numeral. Each digit is
/// comprised of up to three strokes, which are parameterised according to the
/// following bitfield:
///
/// - [7+:1] :: If true, stroke begins at bottom of digit square; otherwise, it
///             begins at the top.
/// - [6+:1] :: If true, stroke begins at right of digit square; otherwise, it
///             begins at the left.
/// - [2+:2] :: Change in row for each render step, sign-extended.
/// - [0+:2] :: Change in column for each render step, sign-extended.
///
/// Strokes are defined relative to the digit square and orientation of the
/// digit representing the hundreds' place, which is in the upper right-hand
/// corner of the overall Cistercian numeral.
static const uint8_t PROGMEM cistercian_strokes[] = {
        //   BR'00'dr'dc
  0x00, // 0b00'00'00'00, no stroke
  0x04, // 0b00'00'01'00, horizontal line at top of digit
  0x84, // 0b10'00'01'00, horizontal line at bottom of digit
  0x41, // 0b01'00'00'01, vertical line at right of digit
  0x05, // 0b00'00'01'01, downward diagonal line
  0x87, // 0b10'00'01'11, upward diagonal line
};

/// Row or column offsets represented by each of the delta fields in a stroke
/// definition as defined above.
static const int8_t PROGMEM cistercian_stroke_offsets[] = { 0, 1, -1, -1 };

/// Stroke definitions for Cistercian digits. Each digit is comprised of up to
/// three strokes, which are represented as a bitfield as illustrated below.
static const uint8_t PROGMEM cistercian_digits[9] = {
  0x10, // 0b0001'00'00
  0x20, // 0b0010'00'00
  0x40, // 0b0100'00'00
  0x50, // 0b0101'00'00
  0x54, // 0b0101'01'00
  0x30, // 0b0011'00'00
  0x1C, // 0b0001'11'00
  0x2C, // 0b0010'11'00
  0x1B, // 0b0001'10'11
};

void cistercian_render_stroke(
    uint8_t stroke, int8_t row, int8_t col, int8_t dr, int8_t dc)
{
  if (!stroke) { return; }

  // Offset the starting row and column to account for the stroke origin.
  row += !!(stroke & CISTERCIAN_STROKE_ROW_ORIGIN_MASK)
      * dr * (CISTERCIAN_STROKE_LENGTH - 1);
  col += !!(stroke & CISTERCIAN_STROKE_COL_ORIGIN_MASK)
      * dc * (CISTERCIAN_STROKE_LENGTH - 1);

  // Reflect the stroke direction according to the row and column deltas.
  dr *= pgm_read_byte(
      cistercian_stroke_offsets + (stroke & CISTERCIAN_STROKE_DELTA_MASK));
  dc *= pgm_read_byte(
      cistercian_stroke_offsets
        + ((stroke >> 2) & CISTERCIAN_STROKE_DELTA_MASK));

  for (int8_t step = 0; step < CISTERCIAN_STROKE_LENGTH; step++) {
    oled_write_pixel(col + step * dc, row + step * dr, 1);
  }
}

void cistercian_render_digit(
    int8_t digit, int8_t row, int8_t col, int8_t dr, int8_t dc)
{
  if (!digit) { return; }

  const uint8_t digit_strokes = pgm_read_byte(cistercian_digits + digit - 1);
  cistercian_render_stroke(
      pgm_read_byte(cistercian_strokes + (digit_strokes & 3)),
      row, col, dr, dc);
  cistercian_render_stroke(
      pgm_read_byte(cistercian_strokes + ((digit_strokes >> 2) & 3)),
      row, col, dr, dc);
  cistercian_render_stroke(
      pgm_read_byte(cistercian_strokes + ((digit_strokes >> 4) & 7)),
      row, col, dr, dc);
}

void cistercian_render(int16_t value, int8_t row, int8_t col)
{
  // Draw the vertical stroke present at the center of every numeral.
  for (int8_t r = CISTERCIAN_PAD_ROWS;
       r < CISTERCIAN_CANVAS_SIZE - CISTERCIAN_PAD_ROWS; r++) {
    oled_write_pixel(
        col + CISTERCIAN_PAD_COLS + CISTERCIAN_STROKE_LENGTH, row + r, 1);
  }

  cistercian_render_digit(
      value % 10,
      row + CISTERCIAN_CANVAS_SIZE - CISTERCIAN_PAD_ROWS - 1,
      col + CISTERCIAN_PAD_COLS + CISTERCIAN_STROKE_LENGTH + 1,
      -1, 1);
  cistercian_render_digit(
      (value / 10) % 10,
      row + CISTERCIAN_CANVAS_SIZE - CISTERCIAN_PAD_ROWS - 1,
      col + CISTERCIAN_PAD_COLS + CISTERCIAN_STROKE_LENGTH - 1,
      -1, -1);
  cistercian_render_digit(
      (value / 100) % 10,
      row + CISTERCIAN_PAD_ROWS,
      col + CISTERCIAN_PAD_COLS + CISTERCIAN_STROKE_LENGTH + 1,
      1, 1);
  cistercian_render_digit(
      (value / 1000) % 10,
      row + CISTERCIAN_PAD_ROWS,
      col + CISTERCIAN_PAD_COLS + CISTERCIAN_STROKE_LENGTH - 1,
      1, -1);
}
