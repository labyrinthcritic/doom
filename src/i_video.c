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
#include <SDL2/SDL_keycode.h>
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
SDL_Texture *screen_texture = NULL;

void I_InitGraphics(void) {
  signal(SIGINT, (void (*)(int))I_Quit);

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Could not initialize SDL2.");
    fprintf(stderr, "%s\n", SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 960, 720, SDL_WINDOW_SHOWN);

  if (window == NULL) {
    fprintf(stderr, "Could not create window.");
    fprintf(stderr, "%s\n", SDL_GetError());
    exit(1);
  }

  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    fprintf(stderr, "Could not create renderer.");
    fprintf(stderr, "%s\n", SDL_GetError());
  }

  screen_texture =
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, SCREENWIDTH, SCREENHEIGHT);
  if (screen_texture == NULL) {
    fprintf(stderr, "Could not create screen texture.");
    fprintf(stderr, "%s\n", SDL_GetError());
  }
}

void I_ShutdownGraphics(void) {
  SDL_DestroyTexture(screen_texture);
  screen_texture = NULL;
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

int TranslateKey(SDL_Keycode code) {
  switch (code) {
  case SDLK_RETURN:
    return KEY_ENTER;
  case SDLK_ESCAPE:
    return KEY_ESCAPE;
  case SDLK_UP:
    return KEY_UPARROW;
  case SDLK_DOWN:
    return KEY_DOWNARROW;
  case SDLK_LEFT:
    return KEY_LEFTARROW;
  case SDLK_RIGHT:
    return KEY_RIGHTARROW;
  case SDLK_LCTRL:
  case SDLK_RCTRL:
    return KEY_RCTRL;
  case SDLK_LSHIFT:
  case SDLK_RSHIFT:
    return KEY_RSHIFT;
  case SDLK_LALT:
  case SDLK_RALT:
    return KEY_RALT;
  case SDLK_TAB:
    return KEY_TAB;
  case SDLK_MINUS:
    return KEY_MINUS;
  case SDLK_EQUALS:
    return KEY_EQUALS;
  case SDLK_SPACE:
    return ' ';
  }

  const char *characters = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ,.";

  for (int i = 0; i < strlen(characters); i++) {
    if (code == characters[i]) {
      return code;
    }
  }

  return -1;
}

void I_FinishUpdate(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      I_Quit();
    } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      int key;
      if ((key = TranslateKey(event.key.keysym.sym)) != -1) {
        event_t doom_event;
        doom_event.type = (event.type == SDL_KEYDOWN ? ev_keydown : ev_keyup);
        doom_event.data1 = key;
        doom_event.data2 = 0;
        doom_event.data3 = 0;
        D_PostEvent(&doom_event);
      }
    }
  }

  SDL_SetRenderDrawColor(renderer, 41, 207, 157, 255);
  SDL_RenderClear(renderer);

  // draw screens[0]

  int *pixels;
  int pitch;
  if (SDL_LockTexture(screen_texture, NULL, (void *)&pixels, &pitch) != 0) {
    fprintf(stderr, "Failed to lock screen texture.");
  }

  for (int i = 0; i < SCREENWIDTH; i++) {
    for (int j = 0; j < SCREENHEIGHT; j++) {
      int pixel_index = (j * SCREENWIDTH + i);

      byte palette_index = screens[0][pixel_index];
      int pixel = 0;
      pixel |= palette[palette_index].blue;
      pixel |= palette[palette_index].green << 8;
      pixel |= palette[palette_index].red << 16;
      pixel |= 255 << 24;
      pixels[pixel_index] = pixel;
      // SDL_SetRenderDrawColor(renderer, palette[palette_index].red,
      //                        palette[palette_index].green,
      //                        palette[palette_index].blue, 255);
      // SDL_RenderDrawPoint(renderer, i, j);
    }
  }

  SDL_UnlockTexture(screen_texture);
  SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
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
