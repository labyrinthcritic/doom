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

#include "d_main.h"
#include "doomstat.h"
#include "i_system.h"
#include "m_argv.h"
#include "v_video.h"

#include "doomdef.h"

void I_InitGraphics(void) {}

void I_ShutdownGraphics(void) {}

void I_StartFrame(void) {
  // er?
}

void I_UpdateNoBlit(void) {
  // what is this?
}

void I_FinishUpdate(void) {}

void I_ReadScreen(byte *scr) {}

void I_SetPalette(byte *palette) {}
