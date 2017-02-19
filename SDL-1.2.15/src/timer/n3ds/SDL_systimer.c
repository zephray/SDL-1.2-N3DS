#include "SDL_config.h"

#include <sys/time.h>
#include <3ds.h>
#include <sys/select.h>


#include "SDL_thread.h"
#include "SDL_timer.h"
#include "SDL_error.h"
#include "../SDL_timer_c.h"

static Uint32 ticksLast, ticksPassed;
static unsigned char UseAccel = 0;
#define ACCEL_RATIO 3

void N3DS_SetAccel(unsigned char acc) {
	UseAccel = (acc)?(ACCEL_RATIO):1;
}

void SDL_StartTicks (void) {
	struct timeval start;
	gettimeofday (&start, NULL);
	ticksLast = start.tv_sec * 1000 + start.tv_usec / 1000;
	ticksPassed = 0;
}

Uint32 SDL_GetTicks (void) {
	Uint32 ticks;
	struct timeval now;

	gettimeofday (&now, NULL);
	ticks = now.tv_sec * 1000 + now.tv_usec / 1000;

	ticksPassed += (ticks - ticksLast) * UseAccel;
	ticksLast = ticks;
	return (ticksPassed);
}

void SDL_Delay (Uint32 ms) {
#ifdef SDL_THREAD_N3DS
	if (threadGetCurrent != NULL)
		svcSleepThread((Uint64)ms * 1000000);
	else {
#endif
	int was_error;
	struct timeval tv;
	Uint32 then, now, elapsed;

	then = SDL_GetTicks();

	do {
		now = SDL_GetTicks ();
		elapsed = (now - then);
		then = now;
		if (elapsed >= ms) {
			break;
		}
		ms -= elapsed;

		tv.tv_sec = ms / 1000;
		tv.tv_usec = (ms % 1000) * 1000;

		was_error = select(0, NULL, NULL, NULL, &tv);
	} while (was_error);
#ifdef SDL_THREAD_N3DS
	}
#endif
}

int SDL_SYS_TimerInit (void) {
	return 0;
}

void SDL_SYS_TimerQuit (void) {
}

int SDL_SYS_StartTimer (void) {
	SDL_SetError ("Timers not implemented on 3DS");

	return -1;
}

void SDL_SYS_StopTimer (void) {
}
