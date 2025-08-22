/**
 * Sega Slide Generator
 *
 * Copyright (c) 2024 Joey Parrish
 *
 * See MIT License in LICENSE.txt
 */

#include <genesis.h>

// A generated file that references all slides.
#include "slides.h"

// Which slide are we showing right now?
static int16_t slide_num;

// Decompress a slide into memory, then flip it to the screen at 0,0 in
// background layers A & B.
static void displayNewSlide() {
  const Image* slide = slides[slide_num];
  const Image* slide_top = slides_top[slide_num];
  PAL_fadeOutAll(1, false);
  VDP_drawImageEx(BG_B, slide, TILE_ATTR_FULL(PAL0, false, false, false, 0),/* x= */ 0, /* y= */ 0, false, false);
  int numTile = slide->tileset->numTile;
  VDP_drawImageEx(BG_A, slide_top, TILE_ATTR_FULL(PAL1, false, false, false, numTile), /* x= */ 0, /* y= */ 0, false, false);
  PAL_fadeToPalette(PAL0,slide->palette->data,1,false);
  PAL_fadeToPalette(PAL1,slide_top->palette->data,1,false);
}

// Handle controller events.
static void onJoystickEvent(u16 joystick, u16 changed, u16 state) {
  // If right was pressed and released since our last check-in, go to the next
  // slide.
  if (state & BUTTON_RIGHT) {
    if (slide_num < num_slides - 1) {
      slide_num++;
      displayNewSlide();
    }
  }
  // If left was pressed, go to the previous slide.
  if (state & BUTTON_LEFT) {
    if (slide_num > 0) {
      slide_num--;
      displayNewSlide();
    }
  }
}

int main(bool hardReset) {
  // Handle controller events.
  JOY_setEventHandler(onJoystickEvent);

  // Display the first slide.
  slide_num = 0;
  displayNewSlide();

  // The standard main loop.  All interesting stuff is triggered by controller
  // events.
  while (true) {
    SYS_doVBlankProcess();
  }

  return 0;
}
