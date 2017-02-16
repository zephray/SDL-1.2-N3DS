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

/* Dummy SDL video driver implementation; this is just enough to make an
 *  SDL-based application THINK it's got a working video driver, for
 *  applications that call SDL_Init(SDL_INIT_VIDEO) when they don't need it,
 *  and also for use as a collection of stubs when porting SDL to a new
 *  platform for which you haven't yet written a valid video driver.
 *
 * This is also a great way to determine bottlenecks: if you think that SDL
 *  is a performance problem for a given platform, enable this driver, and
 *  then see if your application runs faster without video overhead.
 *
 * Initial work by Ryan C. Gordon (icculus@icculus.org). A good portion
 *  of this was cut-and-pasted from Stephane Peter's work in the AAlib
 *  SDL video driver.  Renamed to "DUMMY" by Sam Lantinga.
 */

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include <3ds.h>
#include <citro3d.h>

#include "SDL_n3dsvideo.h"
#include "SDL_n3dsevents_c.h"
#include "SDL_n3dsmouse_c.h"

#define N3DSVID_DRIVER_NAME "N3DS"

static Uint32 n3ds_palette[256] = {0};

/* Initialization/Query functions */
static int N3DS_VideoInit(_THIS, SDL_PixelFormat *vformat);
static SDL_Rect **N3DS_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags);
static SDL_Surface *N3DS_SetVideoMode(_THIS, SDL_Surface *current, int width, int height, int bpp, Uint32 flags);
static int N3DS_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors);
static void N3DS_VideoQuit(_THIS);
static void N3DS_UpdateRects(_THIS, int numrects, SDL_Rect *rects);

/* N3DS driver bootstrap functions */

static int N3DS_Available(void)
{
	return(1); //what else?
}

static void N3DS_DeleteDevice(SDL_VideoDevice *device)
{
	SDL_free(device->hidden);
	SDL_free(device);
}

static SDL_VideoDevice *N3DS_CreateDevice(int devindex)
{
	SDL_VideoDevice *device;

	/* Initialize all variables that we clean on shutdown */
	device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
	if ( device ) {
		SDL_memset(device, 0, (sizeof *device));
		device->hidden = (struct SDL_PrivateVideoData *)
				SDL_malloc((sizeof *device->hidden));
	}
	if ( (device == NULL) || (device->hidden == NULL) ) {
		SDL_OutOfMemory();
		if ( device ) {
			SDL_free(device);
		}
		return(0);
	}
	SDL_memset(device->hidden, 0, (sizeof *device->hidden));
	
	/* Set the function pointers */
	device->VideoInit = N3DS_VideoInit;
	device->ListModes = N3DS_ListModes;
	device->SetVideoMode = N3DS_SetVideoMode;
	device->CreateYUVOverlay = NULL;
	device->SetColors = N3DS_SetColors;
	device->UpdateRects = N3DS_UpdateRects;
	device->VideoQuit = N3DS_VideoQuit;
	device->AllocHWSurface = NULL;
	device->CheckHWBlit = NULL;
	device->FillHWRect = NULL;
	device->SetHWColorKey = NULL;
	device->SetHWAlpha = NULL;
	device->LockHWSurface = NULL;
	device->UnlockHWSurface = NULL;
	device->FlipHWSurface = NULL;
	device->FreeHWSurface = NULL;
	device->SetCaption = NULL;
	device->SetIcon = NULL;
	device->IconifyWindow = NULL;
	device->GrabInput = NULL;
	device->GetWMInfo = NULL;
	device->InitOSKeymap = N3DS_InitOSKeymap;
	device->PumpEvents = N3DS_PumpEvents;

	device->free = N3DS_DeleteDevice;

	return device;
}

VideoBootStrap N3DS_bootstrap = {
	N3DSVID_DRIVER_NAME, "SDL N3DS video driver",
	N3DS_Available, N3DS_CreateDevice
};


int N3DS_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
	// Initialize graphics
	//gfxInitDefault();
	
	vformat->BitsPerPixel = 32;	
	vformat->BytesPerPixel = 4;
	vformat->Rmask = 0xff000000;
	vformat->Gmask = 0x00ff0000;
	vformat->Bmask = 0x0000ff00; 
	vformat->Amask = 0x000000ff; 

	/* We're done! */
	return(0);
}

SDL_Rect **N3DS_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
   	 return (SDL_Rect **) -1;
}

SDL_Surface *N3DS_SetVideoMode(_THIS, SDL_Surface *current,
				int width, int height, int bpp, Uint32 flags)
{
	Uint32 Rmask, Gmask, Bmask, Amask; 

	this->hidden->screens = flags & (SDL_DUALSCR); // SDL_DUALSCR = SDL_TOPSCR | SDL_BOTTOMSCR
	if(this->hidden->screens==0) this->hidden->screens = SDL_TOPSCR; //Default
	flags &= ~SDL_DUALSCR;
	flags |= this->hidden->screens;

	switch(bpp) {
		case 0:
			bpp = 32;
		case 32:
			Rmask = 0xff000000; 
			Gmask = 0x00ff0000;
			Bmask = 0x0000ff00;
			Amask = 0x000000ff;
			this->hidden->mode=GSP_RGBA8_OES;
			this->hidden->byteperpixel=4;
			break;
		case 24:
			Rmask = 0xff0000; 
			Gmask = 0x00ff00;
			Bmask = 0x0000ff;
			Amask = 0x0;
			this->hidden->mode=GSP_BGR8_OES;
			this->hidden->byteperpixel=3;
			break;
		case 16:
			Rmask = 0xF800;
			Gmask = 0x07E0;
			Bmask = 0x001F;
			Amask = 0x0000;
			this->hidden->mode=GSP_RGB565_OES;
			this->hidden->byteperpixel=2;
			break;
		case 8:
			Rmask = 0;
			Gmask = 0;
			Bmask = 0;
			Amask = 0;
			this->hidden->mode=GSP_RGBA8_OES;
			this->hidden->byteperpixel=4;
			break;
		default:
			return NULL;
			break;
	}

	if (this->hidden->buffer) {
		linearFree(this->hidden->buffer);
	}

	this->hidden->buffer = linearAlloc((bpp / 8) * width * height);
	if ( ! this->hidden->buffer ) {
		SDL_SetError("Couldn't allocate buffer for requested mode");
		return(NULL);
	}

	SDL_memset(this->hidden->buffer, 0, (bpp / 8) * width * height);

	//Allocate the new pixel format for the screen
	if ( ! SDL_ReallocFormat(current, bpp, Rmask, Gmask, Bmask, Amask) ) {
		SDL_free(this->hidden->buffer);
		this->hidden->buffer = NULL;
		SDL_SetError("Couldn't allocate new pixel format for requested mode");
		return(NULL);
	}

	//setup the screens mode
	this->hidden->scr_h = 240;
	if (this->hidden->screens & SDL_TOPSCR) {
		gfxInit(this->hidden->mode, GSP_BGR8_OES, false);
		gfxSetDoubleBuffering(GFX_TOP, false);
		this->hidden->scr_w = 400;
		this->hidden->fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);
		if (flags & SDL_CONSOLEBOTTOM) {
			consoleInit(GFX_BOTTOM, NULL);
			this->hidden->console = SDL_CONSOLEBOTTOM;
		}
	} else if(this->hidden->screens & SDL_BOTTOMSCR) {
		gfxInit(GSP_BGR8_OES, this->hidden->mode, false);
		gfxSetDoubleBuffering(GFX_BOTTOM, false);
		this->hidden->scr_w = 320;
		this->hidden->fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
		if (flags & SDL_CONSOLETOP) {
			consoleInit(GFX_TOP, NULL);
			this->hidden->console = SDL_CONSOLETOP;
		}
	}
	
	if ( width < (this->hidden->scr_w) || height < (this->hidden->scr_h) ) {
		int win_x = ((this->hidden->scr_w) - width) / 2;
		int win_y = ((this->hidden->scr_h) - height) / 2;
		this->hidden->offset = (win_x * (this->hidden->scr_h) - win_y) * (this->hidden->byteperpixel);
		this->hidden->win_y = win_y;
	} else
		this->hidden->offset = this->hidden->win_y = 0;

	printf("SDL 1.2.15 for N3DS\n");
 	printf("Setting mode %d %dx%d\n", this->hidden->mode, width, height); 

	//Set up the new mode framebuffer
	current->flags =  SDL_SWSURFACE;
	this->hidden->w = this->info.current_w = current->w = width;
	this->hidden->h = this->info.current_h = current->h = height;
	current->pitch = (bpp / 8) * current->w;
	current->pixels = this->hidden->buffer;
	
	//We're done 
	return(current);
	return NULL;
}

#define N3DS_CopyLoop(code) for (i = 0; i < rows; i++) { \
src_addr = src_base_addr + (rect->x + (i + rect->y) * this->hidden->w ) * srcBytePerPixel; \
dst_addr = dst_base_addr + (this->hidden->scr_h - 1 - (i + rect->y) + rect->x * this->hidden->scr_h) * this->hidden->byteperpixel; \
for (j = 0; j < cols; j++) { \
code \
dst_addr += dst_delta; }}
static void N3DS_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
	int i, j, k;
	Uint32 dst_delta = this->hidden->scr_h * this->hidden->byteperpixel;

	for ( i = 0; i < numrects; ++i ) {
		SDL_Rect *rect = &rects[i];
		Uint8 *src_base_addr, *dst_base_addr;
		Uint8 *src_addr, *dst_addr;
		Uint8 srcBytePerPixel;
		int cols, rows;

		if ( ! rect )
			continue;

		src_base_addr = this->hidden->buffer;
		dst_base_addr = this->hidden->fb;
		dst_base_addr += this->hidden->offset;

		cols = (rect->x + rect->w > this->hidden->w) ? this->hidden->w : rect->w;
		rows = (rect->y + rect->h > this->hidden->h) ? this->hidden->h : rect->h;

		srcBytePerPixel = (SDL_VideoSurface->format->BitsPerPixel) / 8; //this can be different

		if ( SDL_VideoSurface->format->BitsPerPixel == 8 ) {
			N3DS_CopyLoop(*(Uint32 *)dst_addr = n3ds_palette[*(Uint8 *)src_addr]; src_addr++;)
		} else if ( SDL_VideoSurface->format->BitsPerPixel == 32 ) {
			N3DS_CopyLoop(*(Uint32 *)dst_addr = *(Uint32 *)src_addr; src_addr+=4;)
		} else if ( SDL_VideoSurface->format->BitsPerPixel == 24 ) {
			N3DS_CopyLoop(  *(Uint8 *)dst_addr++ = *(Uint8 *)src_addr++;
					*(Uint8 *)dst_addr++ = *(Uint8 *)src_addr++;
					*(Uint8 *)dst_addr++ = *(Uint8 *)src_addr++;
					dst_addr-=3;)
		} else if ( SDL_VideoSurface->format->BitsPerPixel == 16 ) {
			N3DS_CopyLoop(*(Uint16 *)dst_addr = *(Uint16 *)src_addr; src_addr+=2;)
		}
	}
	gfxFlushBuffers();
	gfxSwapBuffers();
}

#define N3DS_MAP_RGB(r, g, b)	((Uint32)r << 24 | (Uint32)g << 16 | (Uint32)b << 8)

static int N3DS_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
	int i;
	for ( i = firstcolor; i < firstcolor + ncolors; ++i )
		n3ds_palette[i] = N3DS_MAP_RGB(colors[i].r, colors[i].g, colors[i].b);
	return(1);
}


/* Note:  If we are terminated, this could be called in the middle of
   another SDL video routine -- notably UpdateRects.
*/
void N3DS_VideoQuit(_THIS)
{
	if (this->screen->pixels != NULL)
	{
		linearFree(this->screen->pixels);
		this->screen->pixels = NULL;
	}
	gfxExit();
}
