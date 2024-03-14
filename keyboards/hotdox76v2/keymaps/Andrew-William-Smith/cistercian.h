// Copyright 2024 Andrew Smith (@Andrew-William-Smith) ::::::::: SOLI DEO GLORIA
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef CISTERCIAN_H_INCLUDED
#define CISTERCIAN_H_INCLUDED

#include <inttypes.h>

#define CISTERCIAN_MODULO 10000
#define CISTERCIAN_STROKE_LENGTH 10
#define CISTERCIAN_CANVAS_SIZE 30
#define CISTERCIAN_PAD_ROWS 2
#define CISTERCIAN_PAD_COLS 2

/// Render the specified value as a Cistercian numeral with its upper left-hand
/// corner at the specified (row, col) coordinates, plus padding as defined in
/// this header.
void cistercian_render(int16_t value, int8_t row, int8_t col);

#endif // CISTERCIAN_H_INCLUDED
