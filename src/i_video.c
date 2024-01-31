// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------

#include <SDL2/SDL.h>
#include <signal.h>

#include "d_main.h"
#include "doomstat.h"
#include "i_system.h"
#include "m_argv.h"
#include "v_video.h"

#include "doomdef.h"

typedef struct {
  byte red;
  byte green;
  byte blue;
} color_t;

// The color palette.
color_t palette[256];

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void I_InitGraphics(void) {
  signal(SIGINT, (void (*)(int))I_Quit);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Could not initialize SDL2.");
    fprintf(stderr, "%s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 320, 200, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    fprintf(stderr, "Could not create window.");
    fprintf(stderr, "%s\n", SDL_GetError());
    exit(1);
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED & SDL_RENDERER_SOFTWARE);

  if (renderer == NULL) {
    fprintf(stderr, "Could not create renderer.");
    fprintf(stderr, "%s\n", SDL_GetError());
  }
}

void I_ShutdownGraphics(void) {
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
  SDL_DestroyWindow(window);
  window = NULL;
  SDL_Quit();
}

void I_StartFrame(void) {
  // er?
}

void I_UpdateNoBlit(void) {
  // what is this?
}

void I_FinishUpdate(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      I_Quit();
    }
  }

  SDL_SetRenderDrawColor(renderer, 41, 207, 157, 255);
  SDL_RenderClear(renderer);

  // draw screens[0]

  for (int i = 0; i < SCREENWIDTH; i++) {
    for (int j = 0; j < SCREENHEIGHT; j++) {
      int pixel_index = (j * SCREENWIDTH + i);

      byte palette_index = screens[0][pixel_index];
      SDL_SetRenderDrawColor(renderer, palette[palette_index].red,
                             palette[palette_index].green,
                             palette[palette_index].blue, 255);
      SDL_RenderDrawPoint(renderer, i, j);
    }
  }

  SDL_RenderPresent(renderer);
}

void I_ReadScreen(byte *scr) {
  memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

void I_SetPalette(byte *pal) {
  for (int i = 0; i < 256; i++) {
    byte red = gammatable[usegamma][*pal++];
    palette[i].red = (red << 8) + red;
    byte green = gammatable[usegamma][*pal++];
    palette[i].green = (green << 8) + green;
    byte blue = gammatable[usegamma][*pal++];
    palette[i].blue = (blue << 8) + blue;
  }
}
