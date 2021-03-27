/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#ifndef _SDL_n3dsvideo_h
#define _SDL_n3dsvideo_h

#include "../SDL_sysvideo.h"

#include <3ds.h>


/* Hidden "this" pointer for the video functions */
#define _THIS	SDL_VideoDevice *this


/* Private display data */

struct SDL_PrivateVideoData {
	int w, h;
	int offset;
	int win_y;
	int scr_w;
	int scr_h;
	void *buffer;
	Uint8 *fb;
	GSPGPU_FramebufferFormat mode;
	unsigned int screens; // SDL_TOPSCR, SDL_BOTTOMSCR, SDL_DUALSCR
	unsigned int console; // SDL_CONSOLETOP, SDL_CONSOLEBOTTOM
	unsigned int fitscreen; // SDL_TRIMBOTTOMSCR, SDL_FITWIDTH, SDL_FITHEIGHT (SDL_FULLSCREEN sets both SDL_FITWIDTH and SDL_FITHEIGHT)
	int byteperpixel;
};

#endif /* _SDL_n3dsvideo_h */

/*
/// Framebuffer format.
typedef enum
{
	GSP_RGBA8_OES=0,   ///< RGBA8. (4 bytes)
	GSP_BGR8_OES=1,    ///< BGR8. (3 bytes)
	GSP_RGB565_OES=2,  ///< RGB565. (2 bytes)
	GSP_RGB5_A1_OES=3, ///< RGB5A1. (2 bytes)
	GSP_RGBA4_OES=4    ///< RGBA4. (2 bytes)
} GSPGPU_FramebufferFormat;
*/