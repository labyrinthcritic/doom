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
  SDL_RenderPresent(renderer);
}

void I_ReadScreen(byte *scr) {}

void I_SetPalette(byte *palette) {}
