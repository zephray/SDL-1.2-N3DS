# SDL-1.2-N3DS
Port of SDL-1.2.15 to Nintendo 3DS based on nop90's port. This port is basiclly working, very compatible with standard SDL but not very efficient. To port some game which require high processing power, one would better look into a fake SDL wrapper layer.

Example codes using this port can be found under examples folder.

# VIDEO
Any video size supported, the video would be rendered centered on the screen. The default screen is the top screen. The drawing is unaccelerated and NOT double-buffered, thus make it possible to port simple old programs without concerning the Vblank or flush the buffer, etc.

32bpp(RGBA8888), 24bpp(RGB888), 16bpp(RGB565) and 8bpp(Palette) modes are supported. (You can now play your favorite palette shifting trick on the 3DS!)

It will always render as SWSURFACE. SDL_FULLSCREEN is of no use, no scailing is supported.

But you may use these custom flags:
- SDL_TOPSCR: select the top screen as the rendering device (default)
- SDL_BOTTOMSCR: select the bottom screen as the rendering device
- SDL_CONSOLETOP: enables console output on the top screen
- SDL_CONSOLEBOTTOM: enables console output on the bottom screen

Currently you may not use both two screens as rendering device. You can use only one screen and another as console output (optional).

# KEY INPUT

default key bindings are:

 KEY_A -> SDLK_a
 KEY_B -> SDLK_b
 KEY_X -> SDLK_x
 KEY_Y -> SDLK-y
 KEY_L -> SDLK_l
 KEY_R -> SDLK_r
 KEY_START -> SDLK_RETURN
 KEY_SELECT -> SDLK_ESCAPE
 KEY_RIGHT -> SDLK_RIGHT
 KEY_LEFT -> SDLK_LEFT
 KEY_UP -> SDLK_UP
 KEY_DOWN -> SDLK_DOWN
 
a custom function is defined to bind one or more N3DS keys to a SDL key value:

 void SDL_N3DSKeyBind(unsigned int hidkey, SDLKey key)

NOTE: circle pad and C-Stick are not mapped to the direction key by default( circle pad is mapped to the default Joystick) but if someone wants to make this mapping can use the following code:
	
	SDL_N3DSKeyBind(KEY_CPAD_UP|KEY_CSTICK_UP, SDLK_UP);
	SDL_N3DSKeyBind(KEY_CPAD_DOWN|KEY_CSTICK_DOWN, SDLK_DOWN);
	SDL_N3DSKeyBind(KEY_CPAD_LEFT|KEY_CSTICK_LEFT, SDLK_LEFT);
	SDL_N3DSKeyBind(KEY_CPAD_DOWN|KEY_CSTICK_DOWN, SDLK_DOWN);

It's not possible to bind a N3DS key to two or more SDL Key values.

# JOYSTICK

Joystick support is enabled but needs testin (and probably fixing)

# MOUSEPOINTER

Mouse pointer is controlled with the touchpad. Touching the bottom screen controls the pointer position and trigger a left button click.

# AUDIO

Audio is working. Audio thread would have a higher priority than the main thread, but it would give main thread a fixed time to process the audio. If you are experiencing problems with the audio, try using a larger sample buffer or change the delay time in SDL_n3dsaudio.c

# MULTITHREAD

Mulitthread is supported. But please bear in mind that due to the design of 3DS' OS, thread won't evenly share CPU time. You would have to use SDL_Delay to give other threads CPU time to run. All threads would be created with a higher priority than the main thread, and they would start running as soon as you create them.

# TIMER

Working in progress.